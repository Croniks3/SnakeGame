#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Food.h"
#include "SG_Food.generated.h"

class UStaticMeshComponent;

UCLASS()
class SNAKEGAME_API ASG_Food : public AActor
{
	GENERATED_BODY()
	
public:	
	ASG_Food();

	void SetModel(const TSharedPtr<SnakeGame::Food>& Food, uint32 CellSize, const SnakeGame::Dimensions& gridDimensions);
	void SetColor(const FLinearColor& Color);

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Origin;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* FoodMesh;

private:
	TWeakPtr<SnakeGame::Food> Food;
	uint32 CellSize;
	SnakeGame::Dimensions GridDimensions;
};