#pragma once

#include "CoreMinimal.h"


namespace Snake
{
	struct Dimensions
	{
		int32 width;
		int32 height;
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