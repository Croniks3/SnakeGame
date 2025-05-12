#include "Framework/SG_Pawn.h"
#include "Camera/CameraComponent.h"


ASG_Pawn::ASG_Pawn()
{
	PrimaryActorTick.bCanEverTick = false;

	Origin = CreateDefaultSubobject<USceneComponent>("Origin");
	check(Origin);
	SetRootComponent(Origin);
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	check(Camera);
	Camera->SetupAttachment(Origin);
}

void ASG_Pawn::UpdateLocation(const Snake::Dimensions& gridDimensions, int32 cellSize, const FTransform& gridOrigin)
{
	const float worldWidth = gridDimensions.width * cellSize;
	const float worldHeight = gridDimensions.height * cellSize;
	
	const float halfAngleRad = FMath::DegreesToRadians(Camera->FieldOfView * 0.5f);
	float z = (worldWidth * 0.5f) / FMath::Tan(halfAngleRad);
	z *= 1.04f;
	const FVector newPawnLocation = gridOrigin.GetLocation() + FVector(0.5f * worldHeight, 0.5f * worldWidth, z);
	SetActorLocation(newPawnLocation);
}