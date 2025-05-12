#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Core/SG_Types.h"
#include "SG_Pawn.generated.h"

class UCameraComponent;

UCLASS()
class SNAKEGAME_API ASG_Pawn : public APawn
{
	GENERATED_BODY()

public:
	ASG_Pawn();
	void UpdateLocation(const Snake::Dimensions& gridDimensions, int32 cellSize, const FTransform& gridOrigin);
	
protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Origin;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;
};
