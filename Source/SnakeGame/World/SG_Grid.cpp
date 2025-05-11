#include "World/SG_Grid.h"
#include "Core/Grid.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogWorldGrid, All, All);


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
	if (grid.IsValid() == false)
	{
		UE_LOG(LogWorldGrid, Fatal, TEXT("Grid is null, game aborted!"));
	}

	GridDimensions = grid.Get()->dimensions();
	CellSize = cellSize;
	WorldWidth = CellSize * GridDimensions.width;
	WorldHeight = CellSize * GridDimensions.height;
}

void ASG_Grid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawGrid();
}

void ASG_Grid::DrawGrid()
{
	for (int32 i = 0; i < GridDimensions.height + 1; ++i)
	{
		const FVector startLocation = GetActorLocation() + GetActorForwardVector() * CellSize * i;
		DrawDebugLine
		(
			GetWorld(),
			startLocation,
			startLocation + GetActorRightVector() * CellSize * WorldWidth,
			FColor::Green,
			false,
			-1.0f,
			0U,
			1.0f
		);
	}
	
	for (int32 i = 0; i < GridDimensions.width + 1; ++i)
	{
		const FVector startLocation = GetActorLocation() + GetActorRightVector() * CellSize * i;
		DrawDebugLine
		(
			GetWorld(),
			startLocation,
			startLocation + GetActorForwardVector() * CellSize * WorldHeight,
			FColor::Red,
			false,
			-1.0f,
			0U,
			1.0f
		);
	}
}