#pragma once

namespace Snake
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
		// Snake
	};

	struct Settings
	{
		Dimensions gridSize;
	};
}