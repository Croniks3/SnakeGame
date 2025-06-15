#include "Core/Grid.h"

DEFINE_LOG_CATEGORY_STATIC(LogGrid, All, All);

//#pragma optimize("", off)

using namespace SnakeGame;


Grid::Grid(const Dimensions& gridSize) // 
	: c_dimensions(Dimensions{gridSize.width + 2, gridSize.height + 2})
{
	m_cells.Init(CellType::EmptyCell, c_dimensions.width * c_dimensions.height);

	initWalls();
	printDebug();
}

void Grid::initWalls()
{
	uint32 height = c_dimensions.height;
	uint32 width = c_dimensions.width;

	for (uint32 y = 0; y < height; y++)
	{
		bool IsFirstOrLastString = y == 0 || y == height - 1;
		for (uint32 x = 0; x < width; x++)
		{
			CellType cellType = IsFirstOrLastString || x == 0 || x == width - 1 ? CellType::WallCell : CellType::EmptyCell;
			m_cells[posToIndex(x, y)] = cellType;
		}
	}
}

void Grid::printDebug()
{
#if !UE_BUILD_SHIPPING
	uint32 height = c_dimensions.height;
	uint32 width = c_dimensions.width;

	for (uint32 y = 0; y < height; y++)
	{
		FString line;
		for (uint32 x = 0; x < width; x++)
		{
			TCHAR symbol;
			switch (m_cells[posToIndex(x, y)])
			{
				case CellType::EmptyCell: 
					symbol = '0'; 
					break;
				case CellType::WallCell:
					symbol = '*';
					break;
				case CellType::SnakeCell:
					symbol = '_';
					break;
			}
			line.AppendChar(symbol).AppendChar(' ');
		}
		UE_LOG(LogGrid, Display, TEXT("%s"), *line);
	}
#endif
}

void SnakeGame::Grid::update(const TSnakeListNode* contentNode, CellType contentType)
{
	auto* currentNode = contentNode;
	while(currentNode != nullptr)
	{
		uint32 index = posToIndex(currentNode->GetValue());
		m_cells[index] = contentType;

		currentNode = currentNode->GetNextNode();
	}
}

uint32 Grid::posToIndex(uint32 x, uint32 y) const
{
	return y * c_dimensions.width + x;
}

uint32 SnakeGame::Grid::posToIndex(const Position& position) const
{
	return posToIndex(position.x, position.y);
}
