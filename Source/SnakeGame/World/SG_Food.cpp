#include "World/SG_Food.h"
#include "Components/StaticMeshComponent.h"
#include "SG_WorldUtils.h"

#define GET_VAR_NAME(var) #var

ASG_Food::ASG_Food()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Origin = CreateDefaultSubobject<USceneComponent>(GET_VAR_NAME(Origin));
	check(Origin);
	SetRootComponent(Origin);

	FoodMesh = CreateDefaultSubobject<UStaticMeshComponent>(GET_VAR_NAME(FoodMesh));
	check(FoodMesh);
	FoodMesh->SetupAttachment(Origin);
}

void ASG_Food::SetModel(const TSharedPtr<SnakeGame::Food>& InFood, uint32 InCellSize, const SnakeGame::Dimensions& InGridDimensions)
{
	Food = InFood;
	CellSize = InCellSize;
	GridDimensions = InGridDimensions;

	SnakeGame::WorldUtils::ScaleMeshByWorldSize(FoodMesh, FVector(CellSize));
}

void ASG_Food::SetColor(const FLinearColor& Color)
{
	if(auto* FoodMaterial = FoodMesh->CreateAndSetMaterialInstanceDynamic(0))
	{
		FoodMaterial->SetVectorParameterValue("Color", Color);
	}
}

void ASG_Food::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Food.IsValid())
	{
		SetActorLocation(SnakeGame::WorldUtils::GridPositionToVector(Food.Pin()->getPosition(), CellSize, GridDimensions));
	}
}