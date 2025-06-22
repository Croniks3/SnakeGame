#include "Framework/SG_GameMode.h"
#include "Core/SG_Types.h"
#include "World/SG_WorldTypes.h"
#include "World/SG_Grid.h"
#include "World/SG_Snake.h"
#include "Core/Grid.h"
#include "Framework/SG_GridPawn.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include <EnhancedInputComponent.h>

//#pragma optimize("", off)

ASG_GameMode::ASG_GameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASG_GameMode::StartPlay()
{
	Super::StartPlay();

	// Init core game
	Game = MakeUnique<SnakeGame::Game>(CreateGameSettings());
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
	check(SnakeVisual);
	SnakeVisual->SetModel(Game->getSnake(), CellSize, Game->getGrid()->dimensions());
	SnakeVisual->FinishSpawning(gridOrigin);

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

	SetupInput();
}

void ASG_GameMode::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	if(Game.IsValid() == true)
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
	if(Value.Get<bool>() == true)
	{
		Input.y = static_cast<int8>(Value.Get<float>());
		Input.x = 0;
	}
}

void ASG_GameMode::OnMoveRight(const FInputActionValue& Value)
{
	if(Value.Get<bool>() == true)
	{
		Input.y = 0;
		Input.x = static_cast<int8>(Value.Get<float>());
	}
}

void ASG_GameMode::OnResetGame(const FInputActionValue& Value)
{
	if(Value.Get<bool>() == true)
	{
		Game.Reset(new SnakeGame::Game(CreateGameSettings()));
		check(Game.IsValid());

		GridVisual->SetModel(Game->getGrid(), CellSize);
		SnakeVisual->SetModel(Game->getSnake(), CellSize, Game->getGrid()->dimensions());
		Input = SnakeGame::SnakeInput{1, 0};
		NextColor();
	}
}

SnakeGame::Settings ASG_GameMode::CreateGameSettings() const
{
	SnakeGame::Settings settings;

	settings.gridSize = SnakeGame::Dimensions{GridSize.X, GridSize.Y};
	settings.snakeSettings.defaultSize = SnakeDefaultSize;
	settings.snakeSettings.startPosition = SnakeGame::Position{GridSize.X / 2 + 1, GridSize.Y / 2 + 1};
	settings.gameSpeed = GameSpeed;

	return settings;
}