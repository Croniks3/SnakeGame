#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SG_Joystick.generated.h"

DECLARE_DELEGATE_OneParam(FJoystickInputUpdateEvent, FVector2D);

class UImage;


UCLASS()
class SNAKEGAME_API USG_Joystick : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** Получить текущий вектор джойстика в диапазоне [-1; 1] по X/Y */
	UFUNCTION(BlueprintCallable, Category = "Joystick")
	FVector2D GetInputVector() const { return InputVector; }

	/** Установить максимальный радиус смещения ручки (в локальных координатах виджета) */
	UFUNCTION(BlueprintCallable, Category = "Joystick")
	void SetMaxRadius(float InRadius) { MaxRadius = FMath::Max(0.0f, InRadius); }

	void NotifyAboutVisibility(ESlateVisibility InVisibility);

	FJoystickInputUpdateEvent InputUpdateEvent;
	
protected:
	// Картинка-подложка для бэкграунда джойстика
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly)
	TObjectPtr<UImage> BaseImage;

	// Картинка кружка самого джойстика
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly)
	TObjectPtr<UImage> ThumbImage;

	/** Максимальный радиус, на который можно увести ручку от центра (в пикселях локальных координат) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joystick")
	float MaxRadius = 100.0f;

	virtual void NativeConstruct() override;
	
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

protected:
	/** Центр джойстика в локальных координатах виджета */
	FVector2D CenterLocal = FVector2D::ZeroVector;

	/** Текущий вектор ввода в диапазоне [-1; 1] по осям X/Y */
	FVector2D InputVector = FVector2D::ZeroVector;

	/** Нажат ли сейчас джойстик (палец на экране или зажата кнопка мыши) */
	bool bIsPressed = false;

	/** Внутренняя логика обновления положения ручки и вектора ввода */
	void UpdateFromPointerPosition(const FGeometry& InGeometry, const FVector2D& ScreenSpacePosition);

	/** Сбросить состояние джойстика (ручка в центр, вектор (0,0)) */
	void ResetJoystick();
};