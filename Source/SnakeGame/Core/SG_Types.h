#pragma once

#include "CoreMinimal.h"
#include "SnakeGame/Containers/List.h"

namespace SnakeGame
{
	struct Dimensions
	{
		uint32 width;
		uint32 height;
	};

	enum class CellType
	{
		EmptyCell = 0,
		WallCell,
		SnakeCell,
		FoodCell
	};

	struct Position
	{
		Position(uint32 inX = 0, uint32 inY = 0) : x(inX), y(inY) {}
		uint32 x;
		uint32 y;

		FORCEINLINE Position& operator+=(const Position& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		FORCEINLINE Position& operator-=(const Position& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		FORCEINLINE Position operator+(const Position& Other) const
		{
			return Position(x + Other.x, y + Other.y);
		}

		FORCEINLINE Position operator-(const Position& Other) const
		{
			return Position(x - Other.x, y - Other.y);
		}

		FORCEINLINE bool operator==(const Position& Other) const
		{
			return x == Other.x && y == Other.y;
		}

		FORCEINLINE bool operator!=(const Position& Other) const
		{
			return !(*this == Other);
		}

		static const Position Zero;
	};
	
	struct SnakeInput
	{
		int8 x;
		int8 y;

		FORCEINLINE bool IsOpossite(const SnakeInput& other) const
		{
			return ((x + other.x) == 0 && x != 0) || ((y + other.y) == 0 && y != 0);
		}

		static const SnakeInput Default;
	};

	struct SnakeSettings
	{
		uint32 defaultSize{4};
		Position startPosition{Position::Zero};
	};

	struct Settings
	{
		Dimensions gridSize{40, 10};
		SnakeSettings snakeSettings;
		float gameSpeed{1.0f};
	};
	
	using TSnakeList = SnakeGame::TDoubleLinkedList<Position>;
	using TSnakeListNode = SnakeGame::TDoubleLinkedList<Position>::TDoubleLinkedListNode;
}