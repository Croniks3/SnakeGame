#pragma once

#include "CoreMinimal.h"
#include "SG_Types.h";


namespace Snake
{
	class Grid
	{
	public:
		Grid(const Dimensions& gridSize);

		Dimensions dimensions() const { return c_dimensions; }

	private:
		const Dimensions c_dimensions;
		TArray<CellType> m_cells;

		void initWalls();
		void printDebug();
		FORCEINLINE int32 posToIndex(int32 x, int32 y) const;
	};
}