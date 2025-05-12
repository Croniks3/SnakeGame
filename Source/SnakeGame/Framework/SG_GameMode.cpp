#include "Framework/SG_GameMode.h"
#include "Core/SG_Types.h"
#include "World/SG_Grid.h"
#include "Core/Grid.h"
#include "Framework/SG_Pawn.h"


void ASG_GameMode::StartPlay()
{
	Super::StartPlay();

	// Init core game
	FInt32Point calculatedGridSize = CalculateGridSize(GridSize.X, GridSize.Y);
	Snake::Settings settings{ calculatedGridSize.X, calculatedGridSize.Y };
	Game = MakeUnique<Snake::Game>(settings);
	check(Game.IsValid());

	// Init world grid
	const FTransform gridOrigin = FTransform::Identity;
	check(GetWorld());
	GridVisual = GetWorld()->SpawnActorDeferred<ASG_Grid>(GridVisualClass, gridOrigin);
	check(GridVisual);

	GridVisual->SetModel(Game->getGrid(), CellSize);
	GridVisual->FinishSpawning(gridOrigin);

	// Set pawn location fitting grid in viewport
	auto* pc = GetWorld()->GetFirstPlayerController();
	check(pc);

	auto* pawn = Cast<ASG_Pawn>(pc->GetPawn());
	check(pawn);
	check(Game->getGrid());
	
	pawn->UpdateLocation(Game->getGrid()->dimensions(), CellSize, gridOrigin);
}

#pragma optimize("", off)
FInt32Point ASG_GameMode::CalculateGridSize(int32 x, int32 y)
{
	FVector2D viewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
	
	float factor = (viewportSize.X / viewportSize.Y) + 0.1f;
	y = (int32)(((x + 2.0f) / factor) - 3.0f);
	return FInt32Point(x, y);
}
#pragma optimize("", off)
