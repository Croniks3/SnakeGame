#include "World/SG_SnakeLink.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "SG_WorldUtils.h"
#include "LoggingConfig.h"

DEFINE_LOG_CATEGORY_STATIC(LogSGSnakeLink, LOG_DEFAULT_VERBOSITY, LOG_COMPILETIME_VERBOSITY);
#define GET_VAR_NAME(var) #var


ASG_SnakeLink::ASG_SnakeLink()
{
	PrimaryActorTick.bCanEverTick = false;

	Origin = CreateDefaultSubobject<USceneComponent>(GET_VAR_NAME(Origin));
	check(Origin);
	SetRootComponent(Origin);

	LinkMesh = CreateDefaultSubobject<UStaticMeshComponent>(GET_VAR_NAME(LinkMesh));
	check(LinkMesh);
	LinkMesh->SetupAttachment(Origin);
}

void ASG_SnakeLink::SetColor(const FLinearColor& Color)
{
	if(auto* LinkMaterial = LinkMesh->CreateAndSetMaterialInstanceDynamic(0))
	{
		LinkMaterial->SetVectorParameterValue("Color", Color);
	}
}

void ASG_SnakeLink::SetScale(uint32 CellSize)
{
	SnakeGame::WorldUtils::ScaleMeshByWorldSize(LinkMesh, FVector(CellSize));
}

void ASG_SnakeLink::Explode()
{
	if(LinkMesh)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation
		(
			GetWorld(),
			ExplosionEffect,
			GetActorLocation()
		);
		SetActorHiddenInGame(true);
	}
	else
	{
		UE_LOG(LogSGSnakeLink, Warning, TEXT("SnakeLink actor is not valid!"));
	}
}