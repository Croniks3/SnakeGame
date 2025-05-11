#include "World/SG_Grid.h"


ASG_Grid::ASG_Grid()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASG_Grid::BeginPlay()
{
	Super::BeginPlay();
}

void ASG_Grid::SetModel(const TSharedPtr<Snake::Grid>& grid, int32 cellSize)
{

}

void ASG_Grid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawGrid();
}

void ASG_Grid::DrawGrid()
{
	
}