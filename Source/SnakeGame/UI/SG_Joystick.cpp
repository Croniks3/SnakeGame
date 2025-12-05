#include "UI/SG_Joystick.h"
#include "Components/Image.h"
#include "LoggingConfig.h"

DEFINE_LOG_CATEGORY_STATIC(LogVirtualJoystick, LOG_DEFAULT_VERBOSITY, LOG_COMPILETIME_VERBOSITY);


void USG_Joystick::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(MaxRadius <= 0.0f)
	{
		MaxRadius = 100.0f;
	}

	// По умолчанию считаем центр в центре BaseImage,
	// если он есть. Иначе — в центре всего виджета.
	if(BaseImage)
	{
		const FGeometry& Geometry = BaseImage->GetCachedGeometry();
		CenterLocal = Geometry.GetLocalSize() * 0.5f;
	}
	else
	{
		const FGeometry& Geometry = GetCachedGeometry();
		CenterLocal = Geometry.GetLocalSize() * 0.5f;
	}

	ResetJoystick();
}

void USG_Joystick::ResetJoystick()
{
	InputVector = FVector2D::ZeroVector;
	bIsPressed = false;

	if(ThumbImage)
	{
		// Сдвигаем ручку в центр (относительно CenterLocal)
		ThumbImage->SetRenderTranslation(FVector2D::ZeroVector);
	}
}

void USG_Joystick::NotifyAboutVisibility(ESlateVisibility InVisibility)
{
	if(InVisibility == ESlateVisibility::Collapsed)
	{
		ResetJoystick();
	}
}

void USG_Joystick::UpdateFromPointerPosition(const FGeometry& InGeometry, const FVector2D& ScreenSpacePosition)
{
	// Переводим позицию указателя из screen space в локальные координаты виджета
	const FVector2D LocalPos = InGeometry.AbsoluteToLocal(ScreenSpacePosition);

	// Смещение от центра
	FVector2D Offset = LocalPos - CenterLocal;

	// Если почти в центре — считаем, что джойстик в нуле
	if(Offset.IsNearlyZero())
	{
		InputVector = FVector2D::ZeroVector;

		if(ThumbImage)
		{
			ThumbImage->SetRenderTranslation(FVector2D::ZeroVector);
		}

		return;
	}

	const float AbsX = FMath::Abs(Offset.X);
	const float AbsY = FMath::Abs(Offset.Y);

	if(AbsX >= AbsY)
	{
		// Двигаемся ТОЛЬКО по X
		const float Sign = FMath::Sign(Offset.X);
		Offset = FVector2D(MaxRadius * Sign, 0.0f);
	}
	else
	{
		// Двигаемся ТОЛЬКО по Y
		const float Sign = FMath::Sign(Offset.Y);
		Offset = FVector2D(0.0f, MaxRadius * Sign);
	}

	// Обновляем положение ручки
	if(ThumbImage)
	{
		ThumbImage->SetRenderTranslation(Offset);
	}

	// Нормализуем вектор ввода в [-1; 1] по активной оси
	InputVector = Offset / MaxRadius;

	// Инвертируем Y, если "вверх" должен быть положительным
	//InputVector.Y = -InputVector.Y;

	if(!InputVector.IsNearlyZero())
	{
		// Уже на одной оси, просто нормализуем до -1 или 1
		if(FMath::Abs(InputVector.X) > FMath::Abs(InputVector.Y))
		{
			InputVector.X = FMath::Sign(InputVector.X);
			InputVector.Y = 0.0f;
		}
		else
		{
			InputVector.Y = FMath::Sign(InputVector.Y);
			InputVector.X = 0.0f;
		}

		// Вызов события обновления Input
		InputUpdateEvent.ExecuteIfBound(InputVector);
	}
}

// ------------------------ TOUCH ------------------------

FReply USG_Joystick::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	bIsPressed = true;

	// Можно обновить CenterLocal по фактической геометрии:
	if(BaseImage)
	{
		const FGeometry& BaseGeometry = BaseImage->GetCachedGeometry();
		CenterLocal = BaseGeometry.GetLocalSize() * 0.5f;
	}
	else
	{
		CenterLocal = InGeometry.GetLocalSize() * 0.5f;
	}

	UpdateFromPointerPosition(InGeometry, InGestureEvent.GetScreenSpacePosition());

	// Сообщаем Slate, что событие обработано
	return FReply::Handled();
}

FReply USG_Joystick::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchMoved(InGeometry, InGestureEvent);

	if(bIsPressed)
	{
		UpdateFromPointerPosition(InGeometry, InGestureEvent.GetScreenSpacePosition());
	}

	return FReply::Handled();
}

FReply USG_Joystick::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	ResetJoystick();

	return FReply::Handled();
}

// ------------------------ MOUSE (для PC/Editor) ------------------------

FReply USG_Joystick::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsPressed = true;

		if(BaseImage)
		{
			const FGeometry& BaseGeometry = BaseImage->GetCachedGeometry();
			CenterLocal = BaseGeometry.GetLocalSize() * 0.5f;
		}
		else
		{
			CenterLocal = InGeometry.GetLocalSize() * 0.5f;
		}

		UpdateFromPointerPosition(InGeometry, InMouseEvent.GetScreenSpacePosition());

		if(TSharedPtr<SWidget> Cached = GetCachedWidget())
		{
			// Захватываем мышь этим виджетом
			return FReply::Handled().CaptureMouse(Cached.ToSharedRef());
		}
		
		return FReply::Handled();;
	}

	return FReply::Unhandled();
}

FReply USG_Joystick::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	if(bIsPressed && InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		UpdateFromPointerPosition(InGeometry, InMouseEvent.GetScreenSpacePosition());
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply USG_Joystick::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	UE_LOG(LogVirtualJoystick, Display, TEXT("(Class = USG_Joystick, Method = NativeOnMouseButtonUp()): Release Left Mouse Button! "))

	if(bIsPressed && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		ResetJoystick();

		if(TSharedPtr<SWidget> Cached = GetCachedWidget())
		{
			return FReply::Handled().ReleaseMouseCapture();
		}

		return FReply::Handled();
	}

	return FReply::Unhandled();
}