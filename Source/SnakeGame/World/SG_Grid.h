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

	void SetModel(const TSharedPtr<Snake::Grid>& grid, int32 cellSize);

private:
	Snake::Dimensions GridDimensions;
	int32 CellSize;
	int32 WorldWidth;
	int32 WorldHeight;

	void DrawGrid();
};
