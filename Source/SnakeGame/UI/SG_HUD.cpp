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
	GameplayWidget->SetVisibility(ESlateVisibility::Visible);

	GameOverWidget = CreateWidget<USG_GameOverWidget>(GetWorld(), GameOverWidgetClass);
	check(GameOverWidget);
	GameOverWidget->AddToViewport();
	GameOverWidget->SetVisibility(ESlateVisibility::Collapsed);
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

	GameOverWidget->SetVisibility(ESlateVisibility::Collapsed);

	Game = InGame;
	GameplayWidget->UpdateScores(InGame->scores());
	
	InGame->subscribeOnGameplayEvent(SnakeGame::FGameplayEvent::FDelegate::CreateLambda
	([this](SnakeGame::GameplayEventType Event)
	{
		switch(Event)
		{
			case SnakeGame::GameplayEventType::FoodTaken:
				if(Game.IsValid())
				{
					GameplayWidget->UpdateScores(Game.Pin()->scores());
				}
				break;
			case SnakeGame::GameplayEventType::GameOver:
				GameOverWidget->SetVisibility(ESlateVisibility::Visible);
				break;
			default:
				break;
		}
	}));
}