#include "World/SG_SnakeLink.h"
#include "Components/StaticMeshComponent.h"


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
	auto staticMesh = LinkMesh->GetStaticMesh();
	check(staticMesh);
	const FBox Box = staticMesh->GetBoundingBox();
	const auto BoxSize = Box.GetSize();

	check(BoxSize.X); check(BoxSize.Y);
	LinkMesh->SetRelativeScale3D(FVector(CellSize / BoxSize.X, CellSize / BoxSize.Y, CellSize / BoxSize.Z));
}
