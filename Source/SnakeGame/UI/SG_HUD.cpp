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
	GameWidgets.Add(EMatchState::GameInProgress, GameplayWidget);

	GameOverWidget = CreateWidget<USG_GameOverWidget>(GetWorld(), GameOverWidgetClass);
	check(GameOverWidget);
	GameWidgets.Add(EMatchState::GameOver, GameOverWidget);

	GameOverWidget->OnRestartClicked.AddUObject(this, &ThisClass::HandleRestartClick);
	GameOverWidget->OnExitClicked.AddUObject(this, &ThisClass::HandleExitClick);

	for(auto& [EMatchState, GameWidget] : GameWidgets)
	{
		if(GameWidget)
		{
			GameWidget->AddToViewport();
			GameWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
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

	SetMatchState(EMatchState::GameInProgress);

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
							/*SetMatchState(EMatchState::GameOver);*/
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

void ASG_HUD::SetMatchState(EMatchState MatchState)
{
	if(CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(GameWidgets.Contains(MatchState))
	{
		CurrentWidget = GameWidgets[MatchState];
		CurrentWidget->SetVisibility(ESlateVisibility::Visible);
	}
}