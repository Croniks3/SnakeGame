#include "World/SG_Grid.h"
#include "Core/Grid.h"
#include "Components/StaticMeshComponent.h"
#include "LoggingConfig.h"

DEFINE_LOG_CATEGORY_STATIC(LogWorldGrid, LOG_DEFAULT_VERBOSITY, LOG_COMPILETIME_VERBOSITY);
#define GET_VAR_NAME(var) #var

ASG_Grid::ASG_Grid()
{
	PrimaryActorTick.bCanEverTick = false;

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

void ASG_Grid::SetModel(const TSharedPtr<SnakeGame::Grid>& grid, uint32 cellSize)
{
	if (grid.IsValid() == false)
	{
		UE_LOG(LogWorldGrid, Fatal, TEXT("Grid is null, game aborted!"));
		return;
	}

	GridDimensions = grid.Get()->dimensions();
	CellSize = cellSize;
	WorldWidth = CellSize * GridDimensions.width;
	WorldHeight = CellSize * GridDimensions.height;

	// Scale and positioning mesh
	auto staticMesh = GridMesh->GetStaticMesh();
	check(staticMesh);
	const FBox Box = staticMesh->GetBoundingBox();
	const auto BoxSize = Box.GetSize();

	check(BoxSize.X); check(BoxSize.Y);
	GridMesh->SetRelativeScale3D(FVector(WorldHeight / BoxSize.X, WorldWidth / BoxSize.Y, 1.0));
	GridMesh->SetRelativeLocation(0.5 * FVector(WorldHeight, WorldWidth, -BoxSize.Z));

	// Setup mesh material
	GridMaterial = GridMesh->CreateAndSetMaterialInstanceDynamic(0);
	if (GridMaterial != nullptr)
	{
		GridMaterial->SetVectorParameterValue("Division", FVector4d(GridDimensions.height, GridDimensions.width, 0, 0));
	}
}

void ASG_Grid::SetColors(FLinearColor& GridBackgroundColor, FLinearColor& GridWallColor, FLinearColor& GridLineColor)
{
	GridMaterial->SetVectorParameterValue("BackgroundColor", GridBackgroundColor);
	GridMaterial->SetVectorParameterValue("WallColor", GridWallColor);
	GridMaterial->SetVectorParameterValue("LineColor", GridLineColor);
}