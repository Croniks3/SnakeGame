#pragma once

#include "CoreMinimal.h"
#include "Core/SG_Types.h"


namespace SnakeGame
{
	class Grid
	{
	public:
		Grid(const Dimensions& gridSize);

		Dimensions dimensions() const { return c_dimensions; }
		void printDebug();
		void update(const TSnakeListNode* contentNode, CellType contentType);
		bool hitTest(const Position& snakeHeadPos, CellType hitWith) const;

	private:
		const Dimensions c_dimensions;
		TArray<CellType> m_cells;
		TMap<CellType, uint32> m_indByType
		{
			{CellType::WallCell, {}},
			{CellType::SnakeCell, {}}
		};

		void initWalls();
		void freeCellByType(CellType cellType);

		FORCEINLINE uint32 posToIndex(uint32 x, uint32 y) const;
		FORCEINLINE uint32 posToIndex(const Position& position) const;
	};
}