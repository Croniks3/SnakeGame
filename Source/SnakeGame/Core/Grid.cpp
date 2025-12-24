#include "Core/Grid.h"
#include "LoggingConfig.h"

DEFINE_LOG_CATEGORY_STATIC(LogGrid, LOG_DEFAULT_VERBOSITY, LOG_COMPILETIME_VERBOSITY);

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
			if(x == 0 || x == width - 1 || y == 0 || y == height - 1)
			{
				const auto index = posToIndex(x, y);
				m_cells[index] = CellType::WallCell;
				m_indByType[CellType::WallCell].Add(index);
			}	
		}
	}
}

void Grid::update(const TSnakeListNode* contentNode, CellType contentType)
{
	freeCellByType(contentType);

	auto* currentNode = contentNode;
	while(currentNode != nullptr)
	{
		updateInternal(currentNode->GetValue(), contentType);
		currentNode = currentNode->GetNextNode();
	}
}

void Grid::update(const Position& position, CellType contentType)
{
	freeCellByType(contentType);
	updateInternal(position, contentType);
}

void SnakeGame::Grid::updateInternal(const Position& position, CellType contentType)
{
	uint32 index = posToIndex(position);
	m_cells[index] = contentType;
	m_indByType[contentType].Add(index);
}

bool Grid::hitTest(const Position& snakeHeadPos, CellType hitWith) const
{
	uint32 index = posToIndex(snakeHeadPos);

	return m_cells[index] == hitWith;
}

bool SnakeGame::Grid::getRandomEmptyPosition(const Position& snakeHeadPos, Position& outRandomPos) const
{
	const uint32 widthWithoutWalls = c_dimensions.width - 2;
	const uint32 heightWithoutWalls = c_dimensions.height - 2;
	const uint32 startX = FMath::RandRange(1, widthWithoutWalls);
	const uint32 startY = FMath::RandRange(1, heightWithoutWalls);
	
	uint32 x = startX, y = startY;
	do
	{
		uint32 index = posToIndex(x, y);
		if(m_cells[index] == CellType::EmptyCell)
		{
			Position randomPosition = indexToPos(index);
			if(randomPosition != snakeHeadPos)
			{
				outRandomPos = randomPosition;
				return true;
			}
		}

		++x;
		if(x > widthWithoutWalls)
		{
			x = 1;
			++y;
			if(y > heightWithoutWalls)
			{
				y = 1;
			}
		}
	}
	while(x != startX || y != startY);
	
	return false;
}

void Grid::freeCellByType(CellType cellType)
{
	TArray<uint32> cellIndices = m_indByType[cellType];
	for(auto& cellIndex : cellIndices)
	{
		m_cells[cellIndex] = CellType::EmptyCell;
	}
	cellIndices.Empty();
}

#if !UE_BUILD_SHIPPING
void Grid::printDebug()
{
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
				case CellType::FoodCell:
					symbol = 'F';
					break;
			}
			line.AppendChar(symbol).AppendChar(' ');
		}
		UE_LOG(LogGrid, Display, TEXT("%s"), *line);
	}
}
#endif

uint32 Grid::posToIndex(uint32 x, uint32 y) const
{
	return y * c_dimensions.width + x;
}

uint32 SnakeGame::Grid::posToIndex(const Position& position) const
{
	return posToIndex(position.x, position.y);
}

Position SnakeGame::Grid::indexToPos(uint32 index) const
{
	uint32 x = index % c_dimensions.width;
	uint32 y = index / c_dimensions.width;
	return Position(x, y);
}