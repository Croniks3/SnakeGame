#include "Framework/SG_GridPawn.h"
#include "Camera/CameraComponent.h"
#include "LoggingConfig.h"

DEFINE_LOG_CATEGORY_STATIC(LogGridPawn, LOG_DEFAULT_VERBOSITY, LOG_COMPILETIME_VERBOSITY);

namespace
{
	double GetHalfFOVTan(double fovDegrees)
	{
		return FMath::Tan(FMath::DegreesToRadians(fovDegrees * 0.5));
	}

	double GetVerticalFOV(double horFovDegrees, double viewportAspect)
	{
		return FMath::RadiansToDegrees(2.0 * FMath::Atan(FMath::Tan(FMath::DegreesToRadians(horFovDegrees) * 0.5) * viewportAspect));
	}

	constexpr double GridMargin = 2.0;
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

void ASG_GridPawn::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		ApplyCameraTimer,
		this,
		&ThisClass::UpdateLocation,
		0.05f,   
		true
	);
}

void ASG_GridPawn::SetGridSettings(const SnakeGame::Dimensions& gridDimensions, uint32 cellSize, const FTransform& gridOrigin)
{
	GridDimensions = gridDimensions;
	CellSize = cellSize;
	GridOrigin = gridOrigin;

	bGridParamsReady = true;
}

void ASG_GridPawn::UpdateLocation()
{
	++ApplyCameraAttempts;

	if(bCameraApplied || ApplyCameraAttempts >= MaxApplyAttempts)
	{
		GetWorldTimerManager().ClearTimer(ApplyCameraTimer);
		return;
	}

	if(!bGridParamsReady)
	{
		return;
	}

	auto GameViewport = GEngine->GameViewport;
	if(!GameViewport)
	{
		return;
	}

	auto* ViewPort = GameViewport->Viewport;
	if (ViewPort == nullptr || ViewPort->GetSizeXY().Y == 0 || GridDimensions.height == 0)
	{
		return;
	}

	const double worldWidth = GridDimensions.width * CellSize;
	const double worldHeight = GridDimensions.height * CellSize;

	double z = 0.0;

	const double viewportAspect = static_cast<double>(ViewPort->GetSizeXY().X) / ViewPort->GetSizeXY().Y;
	const double gridAspect = static_cast<double>(GridDimensions.width) / GridDimensions.height;

	if (viewportAspect <= gridAspect)
	{
		const double marginWorldWidth = worldWidth + GridMargin * CellSize;
		z = marginWorldWidth / GetHalfFOVTan(Camera->FieldOfView);
	}
	else
	{
		check(viewportAspect);
		const double verticalFOV = GetVerticalFOV(Camera->FieldOfView, 1.0 / viewportAspect);
		const double marginWorldHeight = worldHeight + GridMargin * CellSize;
		z = marginWorldHeight / GetHalfFOVTan(verticalFOV);
	}
	
	const FVector newPawnLocation = GridOrigin.GetLocation() + 0.5 * FVector(worldHeight, worldWidth, z);
	SetActorLocation(newPawnLocation);

	bCameraApplied = true;
}