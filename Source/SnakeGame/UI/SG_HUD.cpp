#include "UI/SG_HUD.h"
#include "UI/SG_GameplayWidget.h"
#include "UI/SG_GameOverWidget.h"
#include "UI/SG_StartGameWidget.h"
#include "Core/Game.h"


void ASG_HUD::BeginPlay()
{
	Super::BeginPlay();
	GameplayWidget = CreateWidget<USG_GameplayWidget>(GetWorld(), GameplayWidgetClass);
	check(GameplayWidget);

	GameplayWidget->AddToViewport();
}

void ASG_HUD::SetModel(const TSharedPtr<SnakeGame::Game>& InGame)
{
	if(!InGame)
	{
		return;
	}

	Game = InGame;

	UpdateScores(Game->scores());
	
	InGame->subscribeOnGameplayEvent(SnakeGame::FGameplayEvent::FDelegate::CreateLambda
	([this](SnakeGame::GameplayEventType Event)
	{
		switch(Event)
		{
			case SnakeGame::GameplayEventType::FoodTaken:
				UpdateScores(Game->scores());
				break;
			default:
				break;
		}
	}));
}

void ASG_HUD::SetGameTime(float seconds)
{
	GameplayWidget->SetGameTime(seconds);
}

void ASG_HUD::UpdateScores(uint32 scores)
{
	GameplayWidget->UpdateScores(scores);
}
