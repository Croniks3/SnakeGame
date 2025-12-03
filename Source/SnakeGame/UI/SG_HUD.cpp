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

	GameOverWidget->OnRestartClicked.AddUObject(this, &ThisClass::HandleRestartClick);
	GameOverWidget->OnExitClicked.AddUObject(this, &ThisClass::HandleExitClick);
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

	GameplayWidget->SetVisibility(ESlateVisibility::Visible);
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
				GetWorldTimerManager().SetTimer(
						Timer,
						[this]()
						{
							GameplayWidget->SetVisibility(ESlateVisibility::Collapsed);
							GameOverWidget->SetVisibility(ESlateVisibility::Visible);
							if(Game.IsValid())
							{
								GameOverWidget->SetTotalGameTimeAndScores(Game.Pin()->gameTime(), Game.Pin()->scores());
							}
						},
						1.0f,
						false
					);
				break;
			default:
				break;
		}
	}));
}

void ASG_HUD::HandleRestartClick()
{
	OnRestartClicked.Broadcast();
}

void ASG_HUD::HandleExitClick()
{
	OnExitClicked.Broadcast();
}