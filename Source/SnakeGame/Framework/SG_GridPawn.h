#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Core/SG_Types.h"
#include "SG_GridPawn.generated.h"

class UCameraComponent;

UCLASS()
class SNAKEGAME_API ASG_GridPawn : public APawn
{
	GENERATED_BODY()

public:
	ASG_GridPawn();
	void UpdateLocation(const Snake::Dimensions& gridDimensions, uint32 cellSize, const FTransform& gridOrigin);
	
protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Origin;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

private:
	Snake::Dimensions GridDimensions;
	uint32 CellSize;
	FTransform GridOrigin;
	void OnViewportResized(FViewport* viewPort, uint32 i);
};
