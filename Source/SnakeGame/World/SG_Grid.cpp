#include "World/SG_Grid.h"
#include "Core/Grid.h"
#include "Components/LineBatchComponent.h"
#include "Components/StaticMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWorldGrid, All, All);
#define GET_VAR_NAME(var) #var

ASG_Grid::ASG_Grid()
{
	PrimaryActorTick.bCanEverTick = true;

	Origin = CreateDefaultSubobject<USceneComponent>(GET_VAR_NAME(Origin));
	check(Origin);
	SetRootComponent(Origin);

	GridMesh = CreateDefaultSubobject<UStaticMeshComponent>(GET_VAR_NAME(GridMesh));
	check(GridMesh);
	GridMesh->SetupAttachment(Origin);
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

	check(GridMesh->GetStaticMesh());
	const FBox Box = GridMesh->GetStaticMesh()->GetBoundingBox();
	const auto BoxSize = Box.GetSize();

	check(BoxSize.X); check(BoxSize.Y);
	GridMesh->SetRelativeScale3D(FVector(WorldHeight / BoxSize.X, WorldWidth / BoxSize.Y, 1.0));
	GridMesh->SetRelativeLocation(0.5 * FVector(WorldHeight, WorldWidth, -BoxSize.Z));
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
			1,
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
			1,
			2.0f,
			0
		);
	}
}