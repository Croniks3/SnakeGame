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
	void SetGridSettings(const SnakeGame::Dimensions& gridDimensions, uint32 cellSize, const FTransform& gridOrigin);
	
protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Origin;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	virtual void BeginPlay() override;

private:
	SnakeGame::Dimensions GridDimensions;
	uint32 CellSize;
	FTransform GridOrigin;

	bool bGridParamsReady = false;
	bool bCameraApplied = false;
	uint32 ApplyCameraAttempts = 0;
	uint32 MaxApplyAttempts = 10;
	FTimerHandle ApplyCameraTimer;

	void UpdateLocation();
};
