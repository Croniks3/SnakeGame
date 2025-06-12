#include "Framework/SG_GameMode.h"
#include "Core/SG_Types.h"
#include "World/SG_Grid.h"
#include "Core/Grid.h"
#include "Framework/SG_GridPawn.h"


//#pragma optimize("", off)

void ASG_GameMode::StartPlay()
{
	Super::StartPlay();

	// Init core game
	Snake::Settings settings{ GridSize.X, GridSize.Y };
	Game = MakeUnique<Snake::Game>(settings);
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
}