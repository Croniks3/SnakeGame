#include "Framework/SG_GameMode.h"
#include "Core/SG_Types.h"
#include "World/SG_Grid.h"
#include "Core/Grid.h"
#include "Framework/SG_Pawn.h"


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

	// Set pawn location fitting grid in viewport
	auto* pc = GetWorld()->GetFirstPlayerController();
	check(pc);

	auto* pawn = Cast<ASG_Pawn>(pc->GetPawn());
	check(pawn);
	check(Game->getGrid());

	pawn->UpdateLocation(Game->getGrid()->dimensions(), CellSize, gridOrigin);
}