#pragma once

#include "CoreMinimal.h"
#include "Core/SG_Types.h"

namespace SnakeGame
{
	class WorldUtils
	{
	public:
		static FVector GridPositionToVector(const Position& gridPosition, uint32 cellSize, const Dimensions& gridDimensions)
		{
			return FVector((gridDimensions.height - 1 - gridPosition.y) * cellSize, gridPosition.x * cellSize, 0.0) + FVector(cellSize * 0.5);
		}
	};
}