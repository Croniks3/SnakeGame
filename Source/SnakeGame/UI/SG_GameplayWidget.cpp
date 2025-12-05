#include "UI/SG_GameplayWidget.h"
#include "UI/SG_Joystick.h"
#include "Components/TextBlock.h"
#include "World/SG_WorldUtils.h"


void USG_GameplayWidget::NativeConstruct()
{
	JoystickWidget->InputUpdateEvent.BindUObject(this, &ThisClass::OnInputUpdated);
}

void USG_GameplayWidget::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if(JoystickWidget)
	{
		JoystickWidget->NotifyAboutVisibility(InVisibility);
	}
}

void USG_GameplayWidget::SetGameTime(float seconds)
{
	if(TimeText)
	{
		TimeText->SetText(SnakeGame::WorldUtils::FormatSeconds(seconds));
	}
}

void USG_GameplayWidget::UpdateScores(uint32 scores)
{
	if(ScoresText)
	{
		ScoresText->SetText(SnakeGame::WorldUtils::FormatScores(scores));
	}
}

void USG_GameplayWidget::OnInputUpdated(FVector2D InputVector)
{
	InputUpdateEvent.ExecuteIfBound(InputVector);
}