#include "UI/SG_HUD.h"
#include "UI/SG_GameplayWidget.h"
#include "UI/SG_GameOverWidget.h"
#include "Core/Game.h"


ASG_HUD::ASG_HUD()
{
	PrimaryActorTick.TickInterval = 1.0f;
}

void ASG_HUD::BeginPlay()
{
	Super::BeginPlay();

	GameplayWidget = CreateWidget<USG_GameplayWidget>(GetWorld(), GameplayWidgetClass);
	check(GameplayWidget);
	GameWidgets.Add(ESnakeGameState::GameInProgress, GameplayWidget);

	GameplayWidget->InputUpdateEvent.BindUObject(this, &ThisClass::HandleInputUpdate);

	GameOverWidget = CreateWidget<USG_GameOverWidget>(GetWorld(), GameOverWidgetClass);
	check(GameOverWidget);
	GameWidgets.Add(ESnakeGameState::GameOver, GameOverWidget);

	GameOverWidget->OnRestartClicked.BindUObject(this, &ThisClass::HandleRestartClick);
	GameOverWidget->OnBackClicked.BindUObject(this, &ThisClass::HandleBackClick);

	for(auto& [ESnakeGameState, GameWidget] : GameWidgets)
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

	SetGameState(ESnakeGameState::GameInProgress);

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
							SetGameState(ESnakeGameState::GameOver);
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
	OnRestartClicked.ExecuteIfBound();
}

void ASG_HUD::HandleBackClick()
{
	OnBackClicked.ExecuteIfBound();
}

void ASG_HUD::HandleInputUpdate(FVector2D InputVector)
{
	OnInputUdpated.ExecuteIfBound(InputVector);
}

void ASG_HUD::SetGameState(ESnakeGameState GameState)
{
	if(CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(GameWidgets.Contains(GameState))
	{
		CurrentWidget = GameWidgets[GameState];
		CurrentWidget->SetVisibility(ESlateVisibility::Visible);
	}
}