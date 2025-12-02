#include "UI/SG_GameplayWidget.h"
#include "Components/TextBlock.h"
#include "World/SG_WorldUtils.h"


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