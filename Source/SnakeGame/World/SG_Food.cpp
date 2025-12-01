#include "World/SG_Food.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "SG_WorldUtils.h"
#include "LoggingConfig.h"

DEFINE_LOG_CATEGORY_STATIC(LogSGFood, LOG_DEFAULT_VERBOSITY, LOG_COMPILETIME_VERBOSITY);
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
	SetActorHiddenInGame(false);
}

void ASG_Food::SetColor(const FLinearColor& Color)
{
	FoodColor = Color;
	if(auto* FoodMaterial = FoodMesh->CreateAndSetMaterialInstanceDynamic(0))
	{
		FoodMaterial->SetVectorParameterValue("Color", FoodColor);
	}
}

void ASG_Food::Explode()
{
	if(Food.IsValid())
	{
		if(UNiagaraComponent* NG = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, GetActorLocation()))
		{
			NG->SetVariableLinearColor("ExplosionColor", FoodColor);
		}
	}
	else
	{
		UE_LOG(LogSGFood, Warning, TEXT("Food actor is not valid!"));
	}
}

void ASG_Food::Hide()
{
	SetActorHiddenInGame(true);
}

void ASG_Food::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Food.IsValid())
	{
		SetActorLocation(SnakeGame::WorldUtils::GridPositionToVector(Food.Pin()->getPosition(), CellSize, GridDimensions));
	}
}