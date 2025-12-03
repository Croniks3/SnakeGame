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
	
	if(ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &USG_GameOverWidget::HandleInternalExitClick);
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

void USG_GameOverWidget::HandleInternalRestartClick()
{
	OnRestartClicked.Broadcast();
}

void USG_GameOverWidget::HandleInternalExitClick()
{
	OnExitClicked.Broadcast();
}