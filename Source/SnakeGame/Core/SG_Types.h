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
	
	using TSnakeListNode = TDoubleLinkedList<Position>::TDoubleLinkedListNode;
	class TSnakeList : public TDoubleLinkedList<Position>
	{
	public:
		void MoveTail(TSnakeListNode* Tail, TSnakeListNode* Head, const Position& Pos)
		{
			RemoveNode(Tail);
			InsertNode(Pos, Head->GetNextNode());
		}
	};	
}