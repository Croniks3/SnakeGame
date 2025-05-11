#include "Framework/SG_GameMode.h"
#include "Core/SG_Types.h"


void ASG_GameMode::StartPlay()
{
	Super::StartPlay();

	Snake::Settings settings{GridSize.X, GridSize.Y};
	Game = MakeUnique<Snake::Game>(settings);
}
