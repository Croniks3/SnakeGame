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

	private:
		const Dimensions c_dimensions;
		TArray<CellType> m_cells;

		void initWalls();

		FORCEINLINE uint32 posToIndex(uint32 x, uint32 y) const;
		FORCEINLINE uint32 posToIndex(const Position& position) const;
	};
}