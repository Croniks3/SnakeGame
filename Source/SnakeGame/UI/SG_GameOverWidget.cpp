#include "UI/SG_GameOverWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "World/SG_WorldUtils.h"


void USG_GameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &USG_GameOverWidget::HandleInternalRestartClick);
	}
	
	if(BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &USG_GameOverWidget::HandleInternalBackClick);
	}
}

void USG_GameOverWidget::SetTotalGameTimeAndScores(float Seconds, uint32 Scores)
{
	if(TimeTextValue)
	{
		TimeTextValue->SetText(SnakeGame::WorldUtils::FormatSeconds(Seconds));
	}

	if(ScoresTextValue)
	{
		ScoresTextValue->SetText(SnakeGame::WorldUtils::FormatScores(Scores));
	}
}

void USG_GameOverWidget::SetWidgetLabelText(const FString& Text)
{
	if(GameOverTextLabel)
	{
		GameOverTextLabel->SetText(FText::FromString(Text));
	}
}

void USG_GameOverWidget::HandleInternalRestartClick()
{
	OnRestartClicked.ExecuteIfBound();
}

void USG_GameOverWidget::HandleInternalBackClick()
{
	OnBackClicked.ExecuteIfBound();
}