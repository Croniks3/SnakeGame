#pragma once

namespace SnakeGame
{
	struct Dimensions
	{
		uint32 width;
		uint32 height;
	};

	enum CellType
	{
		Empty = 0,
		Wall = 1,
		// Food
		// SnakeGame
	};

	struct Settings
	{
		Dimensions gridSize;
	};
}