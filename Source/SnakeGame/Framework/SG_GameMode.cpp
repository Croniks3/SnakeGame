#include "Framework/SG_GameMode.h"
#include "Core/SG_Types.h"
#include "World/SG_WorldTypes.h"
#include "World/SG_Grid.h"
#include "World/SG_Snake.h"
#include "World/SG_Food.h"
#include "UI/SG_HUD.h"
#include "Core/Grid.h"
#include "LoggingConfig.h"
#include "Framework/SG_GridPawn.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include <EnhancedInputComponent.h>

DEFINE_LOG_CATEGORY_STATIC(LogSGGameMode, LOG_DEFAULT_VERBOSITY, LOG_COMPILETIME_VERBOSITY);

//#pragma optimize("", off)

ASG_GameMode::ASG_GameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASG_GameMode::StartPlay()
{
	Super::StartPlay();

	// Init core game
	Game = MakeShared<SnakeGame::Game>(CreateGameSettings());
	check(Game.IsValid());
	
	// Init world grid
	const FTransform gridOrigin = FTransform::Identity;
	check(GetWorld());
	GridVisual = GetWorld()->SpawnActorDeferred<ASG_Grid>(GridVisualClass, gridOrigin);
	check(GridVisual);
	GridVisual->SetModel(Game->getGrid(), CellSize);
	GridVisual->FinishSpawning(gridOrigin);

	// Init world snake
	SnakeVisual = GetWorld()->SpawnActorDeferred<ASG_Snake>(SnakeVisualClass, gridOrigin);
	SnakeVisual->SetModel(Game->getSnake(), CellSize, Game->getGrid()->dimensions());
	SnakeVisual->FinishSpawning(gridOrigin);

	// Init world food
	FoodVisual = GetWorld()->SpawnActorDeferred<ASG_Food>(FoodVisualClass, gridOrigin);
	FoodVisual->SetModel(Game->getFood(), CellSize, Game->getGrid()->dimensions());
	FoodVisual->FinishSpawning(gridOrigin);

	// Set gridPawn location fitting grid in viewport
	auto* pc = GetWorld()->GetFirstPlayerController();
	check(pc);

	ASG_GridPawn* gridPawn = Cast<ASG_GridPawn>(pc->GetPawn());
	if (gridPawn == nullptr && GetWorld()->IsEditorWorld() == true)
	{
		gridPawn = GetWorld()->SpawnActor<ASG_GridPawn>();
		pc->SetPawn(gridPawn);
	}
	check(gridPawn);
	check(Game->getGrid());

	gridPawn->UpdateLocation(Game->getGrid()->dimensions(), CellSize, gridOrigin);

	FindFog();

	// Update colors
	check(ColorsTable);
	const auto RowsCount = ColorsTable->GetRowNames().Num();
	check(RowsCount > 0);
	ColorTableIndex = FMath::RandRange(0, RowsCount - 1);
	UpdateColors();

	// Input
	SetupInput();

	// Set HUD
	HUD = Cast<ASG_HUD>(pc->GetHUD());
	check(HUD);
	HUD->SetModel(Game);

	// Events
	SubscribeOnGameEvents();
	SubscribeOnHUDEvents();

	// Mouse
	pc->bShowMouseCursor = true;
	pc->bEnableClickEvents = true;
	pc->bEnableMouseOverEvents = true;

	// Режим ввода: Game + UI
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);                          
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);  

	pc->SetInputMode(InputMode);
}

void ASG_GameMode::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	if(Game.IsValid() == true && Game->isGameOver() == false)
	{
		Game->update(deltaSeconds, Input);
	}
}

void ASG_GameMode::UpdateColors()
{
	const auto RowName = ColorsTable->GetRowNames()[ColorTableIndex];
	FSnakeColors* colorsSet = ColorsTable->FindRow<FSnakeColors>(RowName, {});
	if (colorsSet != nullptr)
	{
		GridVisual->SetColors(colorsSet->GridBackgroundColor, colorsSet->GridWallColor, colorsSet->GridLineColor);
		SnakeVisual->SetColors(colorsSet->SnakeHeadColor, colorsSet->SnakeLinkColor);
		FoodVisual->SetColor(colorsSet->FoodColor);

		if (Fog != nullptr && Fog->GetComponent() != nullptr)
		{
			Fog->GetComponent()->SetSkyAtmosphereAmbientContributionColorScale(colorsSet->SkyAtmosphereColor);
			Fog->GetComponent()->MarkRenderStateDirty();
		}
	}
}

void ASG_GameMode::FindFog()
{
	TArray<AActor*> Fogs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AExponentialHeightFog::StaticClass(), Fogs);
	if (Fogs.Num() > 0)
	{
		Fog = Cast<AExponentialHeightFog>(Fogs[0]);
	}
}

void ASG_GameMode::NextColor()
{
	if (ColorsTable != nullptr)
	{
		ColorTableIndex = (ColorTableIndex + 1) % ColorsTable->GetRowNames().Num();
		UpdateColors();
	}
}

void ASG_GameMode::SetupInput()
{
	if(GetWorld() == nullptr) { return; }
	auto* pc = GetWorld()->GetFirstPlayerController();
	if(pc != nullptr)
	{
		if(auto* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer()))
		{
			InputSystem->AddMappingContext(SnakeInputMappingContext, 0);
		}

		auto* enhancedInput = Cast<UEnhancedInputComponent>(pc->InputComponent);
		check(enhancedInput);
		enhancedInput->BindAction(MoveForwardInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnMoveForward);
		enhancedInput->BindAction(MoveRightInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnMoveRight);
		enhancedInput->BindAction(ResetGameInputAction, ETriggerEvent::Started, this, &ThisClass::OnResetGame);
	}
}

void ASG_GameMode::OnMoveForward(const FInputActionValue& Value)
{
	const float InputValue = Value.Get<float>();
	if(InputValue == 0.0f) { return; }
	Input.x = 0;
	Input.y = static_cast<int8>(InputValue);
}

void ASG_GameMode::OnMoveRight(const FInputActionValue& Value)
{
	const float InputValue = Value.Get<float>();
	if(InputValue == 0.0f) { return; }
	Input.x = static_cast<int8>(InputValue);
	Input.y = 0;
}

void ASG_GameMode::OnResetGame(const FInputActionValue& Value)
{
	if(Value.Get<bool>() == true)
	{
		ResetGameInternal();
	}
}

void ASG_GameMode::ResetGameInternal()
{
	Game = MakeShared<SnakeGame::Game>(CreateGameSettings());
	check(Game.IsValid());
	SubscribeOnGameEvents();

	GridVisual->SetModel(Game->getGrid(), CellSize);
	SnakeVisual->SetModel(Game->getSnake(), CellSize, Game->getGrid()->dimensions());
	FoodVisual->SetModel(Game->getFood(), CellSize, Game->getGrid()->dimensions());
	HUD->SetModel(Game);
	Input = SnakeGame::SnakeInput::Default;
	NextColor();
}

SnakeGame::Settings ASG_GameMode::CreateGameSettings() const
{
	SnakeGame::Settings settings;

	settings.gridSize = SnakeGame::Dimensions{GridSize.X, GridSize.Y};
	settings.snakeSettings.defaultSize = SnakeDefaultSize;
	settings.snakeSettings.startPosition = SnakeGame::Grid::center(GridSize.X, GridSize.Y);
		/*SnakeGame::Position{GridSize.X / 2 + 1, GridSize.Y / 2 + 1}*/;
	settings.gameSpeed = GameSpeed;

	return settings;
}

void ASG_GameMode::ExitGame()
{
	UWorld* World = GetWorld();
	if(!World)
	{
		return;
	}

	auto* PC = World->GetFirstPlayerController();
		
	UKismetSystemLibrary::QuitGame(
		World,
		PC,
		EQuitPreference::Quit,  
		false                   
	);
}

void ASG_GameMode::SubscribeOnGameEvents()
{
	Game->subscribeOnGameplayEvent(SnakeGame::FGameplayEvent::FDelegate::CreateLambda
		([this](SnakeGame::GameplayEventType Event)
		{
			switch(Event)
			{
				case SnakeGame::GameplayEventType::GameOver:
					UE_LOG(LogSGGameMode, Display, TEXT("------------- GAME OVER! ------------- "));
					UE_LOG(LogSGGameMode, Display, TEXT("------------- SCORES: %i! ------------- "), Game->scores());

					SnakeVisual->Explode();
					FoodVisual->Hide();

					break;
				case SnakeGame::GameplayEventType::GameCompleted:
					UE_LOG(LogSGGameMode, Display, TEXT("------------- GAME COMPLETE! ------------- "));
					UE_LOG(LogSGGameMode, Display, TEXT("------------- SCORES: %i! ------------- "), Game->scores());

					FoodVisual->Hide();

					break;
				case SnakeGame::GameplayEventType::FoodTaken:
					UE_LOG(LogSGGameMode, Display, TEXT("------------- FOOD TAKEN! ------------- "));

					FoodVisual->Explode();

					break;
				default:
					break;
			}
		}));
}

void ASG_GameMode::SubscribeOnHUDEvents()
{
	HUD->OnRestartClicked.AddLambda([this]()
	{
		ResetGameInternal();
	});

	HUD->OnExitClicked.AddLambda([this]()
	{
		ExitGame();
	});
}