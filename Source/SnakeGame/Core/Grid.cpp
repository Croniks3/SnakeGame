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

Position SnakeGame::Grid::getRandomEmptyPosition() const
{
	const auto gridSize = c_dimensions.height * c_dimensions.width;
	const auto randIndex = FMath::RandRange(0, gridSize - 1);
	
	for(uint32 i = randIndex; i < gridSize; ++i)
	{
		if(m_cells[i] == CellType::EmptyCell)
		{
			return indexToPos(i);
		}
	}
	
	for(int32 i = 0; i < randIndex; ++i)
	{
		if(m_cells[i] == CellType::EmptyCell)
		{
			return indexToPos(i);
		}
	}

	UE_LOG(LogGrid, Error, TEXT("Empty cell dosn't exist!"));
	return Position(-1, -1);
}

Position SnakeGame::Grid::getRandomEmptyPosition(const Position& snakeHeadPos) const
{
	const auto gridSize = c_dimensions.height * c_dimensions.width;
	const auto randIndex = FMath::RandRange(0, gridSize - 1);

	for(uint32 i = randIndex; i < gridSize; ++i)
	{
		if(m_cells[i] == CellType::EmptyCell)
		{
			Position randomPosition = indexToPos(i);
			if(randomPosition != snakeHeadPos)
			{
				return randomPosition;
			}	
		}
	}

	for(int32 i = 0; i < randIndex; ++i)
	{
		if(m_cells[i] == CellType::EmptyCell)
		{
			Position randomPosition = indexToPos(i);
			if(randomPosition != snakeHeadPos)
			{
				return randomPosition;
			}
		}
	}

	UE_LOG(LogGrid, Error, TEXT("Empty cell dosn't exist!"));
	return Position(-1, -1);
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
				case CellType::FoodCell:
					symbol = 'F';
					break;
			}
			line.AppendChar(symbol).AppendChar(' ');
		}
		UE_LOG(LogGrid, Display, TEXT("%s"), *line);
	}
#endif
}

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