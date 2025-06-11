#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/SG_Types.h"

#include "SG_Grid.generated.h"

namespace Snake
{
	class Grid;
}

UCLASS()
class SNAKEGAME_API ASG_Grid : public AActor
{
	GENERATED_BODY()

public:
	ASG_Grid();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SetModel(const TSharedPtr<Snake::Grid>& grid, uint32 cellSize);

private:
	Snake::Dimensions GridDimensions;
	uint32 CellSize;
	uint32 WorldWidth;
	uint32 WorldHeight;

	void DrawGrid();
};
