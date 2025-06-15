#pragma once

#include "CoreMinimal.h"
#include "Containers/List.h"

namespace SnakeGame
{
	struct Dimensions
	{
		uint32 width;
		uint32 height;
	};

	enum CellType
	{
		EmptyCell = 0,
		WallCell,
		SnakeCell,
		// Food
	};

	struct Position
	{
		Position(uint32 inX, uint32 inY) : x(inX), y(inY) {}
		uint32 x;
		uint32 y;

		FORCEINLINE Position& operator+=(const Position& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}
	};
	
	struct SnakeInput
	{
		int8 x;
		int8 y;
	};

	struct SnakeSettings
	{
		uint32 defaultSize{4};
		Position startPosition{0, 0};
	};

	struct Settings
	{
		Dimensions gridSize{40, 10};
		SnakeSettings snakeSettings;
	};

	using TSnakeList = TDoubleLinkedList<Position>;
	using TSnakeListNode = TSnakeList::TDoubleLinkedListNode;
}