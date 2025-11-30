#pragma once

#include "CoreMinimal.h"
#include "SG_Types.h"

namespace SnakeGame
{
	class IPositionRandomizer
	{
	public:
		virtual bool generateRandomPosition(const Dimensions& dimensions, const TArray<CellType>& cells, const Position& snakeHeadPos, Position& outRandomPos) const = 0;
	};

	class PositionRandomizer : public IPositionRandomizer
	{
	public:
		virtual bool generateRandomPosition(const Dimensions& dimensions, const TArray<CellType>& cells, const Position& snakeHeadPos, Position& outRandomPos) const override
		{
			const auto gridSize = dimensions.height * dimensions.width;
			const auto randIndex = FMath::RandRange(0, gridSize - 1);

			for(uint32 i = randIndex; i < gridSize; ++i)
			{
				if(cells[i] == CellType::EmptyCell)
				{
					Position randomPosition = indexToPos(i, dimensions);
					if(randomPosition != snakeHeadPos)
					{
						outRandomPos = randomPosition;
						return true;
					}
				}
			}

			for(int32 i = 0; i < randIndex; ++i)
			{
				if(cells[i] == CellType::EmptyCell)
				{
					Position randomPosition = indexToPos(i, dimensions);
					if(randomPosition != snakeHeadPos)
					{
						outRandomPos = randomPosition;
						return true;
					}
				}
			}

			return false;
		}

	private:
		FORCEINLINE Position indexToPos(uint32 index, const Dimensions& dimensions) const
		{
			uint32 x = index % dimensions.width;
			uint32 y = index / dimensions.width;
			return Position(x, y);
		}
	};
}