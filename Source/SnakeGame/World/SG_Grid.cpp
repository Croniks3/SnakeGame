#include "World/SG_Grid.h"
#include "Core/Grid.h"
#include "DrawDebugHelpers.h"
#include "Components/LineBatchComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWorldGrid, All, All);


ASG_Grid::ASG_Grid()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASG_Grid::BeginPlay()
{
	Super::BeginPlay();
}

void ASG_Grid::SetModel(const TSharedPtr<Snake::Grid>& grid, uint32 cellSize)
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
	if (GetWorld() == nullptr || GetWorld()->GetLineBatcher(UWorld::ELineBatcherType::World) == nullptr)
	{
		return;
	}

	for (uint32 i = 0; i < GridDimensions.height + 1; ++i)
	{
		const FVector startLocation = GetActorLocation() + GetActorForwardVector() * CellSize * i;
		GetWorld()->GetLineBatcher(UWorld::ELineBatcherType::World)->DrawLine
		(
			startLocation,
			startLocation + GetActorRightVector() * WorldWidth,
			FLinearColor::Green,
			0,
			2.0f,
			0
		);
	}
	
	for (uint32 i = 0; i < GridDimensions.width + 1; ++i)
	{
		const FVector startLocation = GetActorLocation() + GetActorRightVector() * CellSize * i;
		GetWorld()->GetLineBatcher(UWorld::ELineBatcherType::World)->DrawLine
		(
			startLocation,
			startLocation + GetActorForwardVector() * WorldHeight,
			FLinearColor::Red,
			0,
			2.0f,
			0
		);
	}
}