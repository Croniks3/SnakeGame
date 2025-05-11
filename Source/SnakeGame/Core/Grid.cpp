#include "Core/Grid.h"

DEFINE_LOG_CATEGORY_STATIC(LogGrid, All, All);

using namespace Snake;


Grid::Grid(const Dimensions& gridSize) // 
	: c_dimensions(Dimensions{gridSize.width + 2, gridSize.height + 2})
{
	m_cells.Init(CellType::Empty, c_dimensions.width * c_dimensions.height);

	initWalls();
	printDebug();
}

void Grid::initWalls()
{
	int32 height = c_dimensions.height;
	int32 width = c_dimensions.width;

	for (int32 y = 0; y < height; y++)
	{
		bool IsFirstOrLastString = y == 0 || y == height - 1;
		for (int32 x = 0; x < width; x++)
		{
			CellType cellType = IsFirstOrLastString || x == 0 || x == width - 1 ? CellType::Wall : CellType::Empty;
			m_cells[posToIndex(x, y)] = cellType;
		}
	}
}

void Grid::printDebug()
{
#if !UE_BUILD_SHIPPING
	int32 height = c_dimensions.height;
	int32 width = c_dimensions.width;

	for (int32 y = 0; y < height; y++)
	{
		FString line;
		for (int32 x = 0; x < width; x++)
		{
			TCHAR symbol;
			switch (m_cells[posToIndex(x, y)])
			{
				case CellType::Empty: 
					symbol = '0'; 
					break;
				case CellType::Wall:
					symbol = '*';
					break;
			}
			line.AppendChar(symbol).AppendChar(' ');
		}
		UE_LOG(LogGrid, Display, TEXT("%s"), *line);
	}
#endif
}

int32 Grid::posToIndex(int32 x, int32 y) const
{
	return y * c_dimensions.width + x;
}
