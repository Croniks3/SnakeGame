#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/SG_Types.h"

#include "SG_Grid.generated.h"

namespace SnakeGame
{
	class Grid;
}

class UStaticMeshComponent;

UCLASS()
class SNAKEGAME_API ASG_Grid : public AActor
{
	GENERATED_BODY()

public:
	ASG_Grid();
	void SetModel(const TSharedPtr<SnakeGame::Grid>& grid, uint32 cellSize);
	void SetColors(FLinearColor& GridBackgroundColor, FLinearColor& GridWallColor, FLinearColor& GridLineColor);

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Origin;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* GridMesh;

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UMaterialInstanceDynamic* GridMaterial;

	SnakeGame::Dimensions GridDimensions;
	uint32 CellSize;
	uint32 WorldWidth;
	uint32 WorldHeight;
};