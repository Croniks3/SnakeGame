#include "Framework/SG_GridPawn.h"
#include "Camera/CameraComponent.h"

namespace
{
	float GetHalfFOVTan(float fovDegrees)
	{
		return FMath::Tan(FMath::DegreesToRadians(fovDegrees * 0.5f));
	}

	float GetVerticalFOV(float horFovDegrees, float viewportAspect)
	{
		return FMath::RadiansToDegrees(2.0f * FMath::Atan(FMath::Tan(FMath::DegreesToRadians(horFovDegrees) * 0.5f) * viewportAspect));
	}
}

ASG_GridPawn::ASG_GridPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	Origin = CreateDefaultSubobject<USceneComponent>("Origin");
	check(Origin);
	SetRootComponent(Origin);
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	check(Camera);
	Camera->SetupAttachment(Origin);
}

void ASG_GridPawn::UpdateLocation(const Snake::Dimensions& gridDimensions, uint32 cellSize, const FTransform& gridOrigin)
{
	GridDimensions = gridDimensions;
	CellSize = cellSize;
	GridOrigin = gridOrigin;

	check(GEngine);
	check(GEngine->GameViewport);
	check(GEngine->GameViewport->Viewport);

	auto* viewPort = GEngine->GameViewport->Viewport;
	viewPort->ViewportResizedEvent.AddUObject(this, &ThisClass::OnViewportResized);

#if WITH_EDITOR
	OnViewportResized(viewPort, 0);
#endif
}

void ASG_GridPawn::OnViewportResized(FViewport* viewPort, uint32 i)
{
	if (viewPort == nullptr || viewPort->GetSizeXY().Y == 0 || GridDimensions.height == 0)
	{
		return;
	}

	const float worldWidth = GridDimensions.width * CellSize;
	const float worldHeight = GridDimensions.height * CellSize;

	float z = 0.0f;

	const float viewportAspect = static_cast<float>(viewPort->GetSizeXY().X) / viewPort->GetSizeXY().Y;
	const float gridAspect = static_cast<float>(GridDimensions.width) / GridDimensions.height;

	if (viewportAspect <= gridAspect)
	{
		z = worldWidth / GetHalfFOVTan(Camera->FieldOfView);
	}
	else
	{
		check(viewportAspect);
		const float verticalFOV = GetVerticalFOV(Camera->FieldOfView, 1.0f / viewportAspect);
		z = worldHeight / GetHalfFOVTan(verticalFOV);
	}
	z *= 1.04f;

	const FVector newPawnLocation = GridOrigin.GetLocation() + 0.5f * FVector(worldHeight, worldWidth, z);
	SetActorLocation(newPawnLocation);
}