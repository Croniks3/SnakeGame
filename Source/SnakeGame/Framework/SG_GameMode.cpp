#include "Framework/SG_GameMode.h"
#include "Core/SG_Types.h"
#include "World/SG_Grid.h"


void ASG_GameMode::StartPlay()
{
	Super::StartPlay();

	Snake::Settings settings{GridSize.X, GridSize.Y};
	Game = MakeUnique<Snake::Game>(settings);
	check(Game.IsValid());

	const FTransform gridOrigin = FTransform::Identity;
	check(GetWorld());
	GridVisual = GetWorld()->SpawnActorDeferred<ASG_Grid>(GridVisualClass, gridOrigin);
	check(GridVisual);

	GridVisual->SetModel(Game->getGrid(), CellSize);
	GridVisual->FinishSpawning(gridOrigin);
}
