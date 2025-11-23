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
		void update(const Position& position, CellType contentType);
		bool hitTest(const Position& snakeHeadPos, CellType hitWith) const;

		Position getRandomEmptyPosition() const;
		Position getRandomEmptyPosition(const Position& snakeHeadPos) const;

		static Position center(uint32 width, uint32 height) { return Position{width / 2 + 1, height / 2 + 1}; }

	private:
		const Dimensions c_dimensions;
		TArray<CellType> m_cells;
		TMap<CellType, TArray<uint32>> m_indByType
		{
			{CellType::WallCell, {}},
			{CellType::SnakeCell, {}},
			{CellType::FoodCell, {}}
		};

		void initWalls();
		void freeCellByType(CellType cellType);
		void updateInternal(const Position& position, CellType contentType);

		FORCEINLINE uint32 posToIndex(uint32 x, uint32 y) const;
		FORCEINLINE uint32 posToIndex(const Position& position) const;
		FORCEINLINE Position indexToPos(uint32 index) const;
	};
}