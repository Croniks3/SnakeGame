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

void ASG_HUD::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	if(Game.IsValid() == true && Game.Pin()->isGameOver() == false)
	{
		GameplayWidget->SetGameTime(Game.Pin()->gameTime());
	}
}

void ASG_HUD::SetModel(const TSharedPtr<SnakeGame::Game>& InGame)
{
	if(!InGame)
	{
		return;
	}

	Game = InGame;
	GameplayWidget->UpdateScores(InGame->scores());
	
	InGame->subscribeOnGameplayEvent(SnakeGame::FGameplayEvent::FDelegate::CreateLambda
	([&](SnakeGame::GameplayEventType Event)
	{
		switch(Event)
		{
			case SnakeGame::GameplayEventType::FoodTaken:
				GameplayWidget->UpdateScores(InGame->scores());
				break;
			default:
				break;
		}
	}));
}