#include "Framework/SG_GameMode.h"
#include "Core/SG_Types.h"
#include "World/SG_WorldTypes.h"
#include "World/SG_Grid.h"
#include "Core/Grid.h"
#include "Framework/SG_GridPawn.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Kismet/GameplayStatics.h"


//#pragma optimize("", off)

void ASG_GameMode::StartPlay()
{
	Super::StartPlay();

	// Init core game
	SnakeGame::Settings settings{ GridSize.X, GridSize.Y };
	Game = MakeUnique<SnakeGame::Game>(settings);
	check(Game.IsValid());

	// Init world grid
	const FTransform gridOrigin = FTransform::Identity;
	check(GetWorld());
	GridVisual = GetWorld()->SpawnActorDeferred<ASG_Grid>(GridVisualClass, gridOrigin);
	check(GridVisual);

	GridVisual->SetModel(Game->getGrid(), CellSize);
	GridVisual->FinishSpawning(gridOrigin);

	// Set gridPawn location fitting grid in viewport
	auto* pc = GetWorld()->GetFirstPlayerController();
	check(pc);

	auto* gridPawn = Cast<ASG_GridPawn>(pc->GetPawn());
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
}

void ASG_GameMode::UpdateColors()
{
	const auto RowName = ColorsTable->GetRowNames()[ColorTableIndex];
	FSnakeColors* colorsSet = ColorsTable->FindRow<FSnakeColors>(RowName, {});
	if (colorsSet != nullptr)
	{
		GridVisual->SetColors(colorsSet->GridBackgroundColor, colorsSet->GridWallColor, colorsSet->GridLineColor);

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