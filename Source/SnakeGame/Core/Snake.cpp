#include "Core/Snake.h"

using namespace SnakeGame;


Snake::Snake(const SnakeSettings& snakeSettings)
{
	checkf(snakeSettings.defaultSize >= 4, TEXT("Snake length is to small: %i"), snakeSettings.defaultSize);

	// Add snake links horizontaly to the lef [ ---* ]
	const auto startPos = snakeSettings.startPosition;
	for(uint32 i = 0; i < snakeSettings.defaultSize; ++i)
	{
		m_links.AddTail(Position{startPos.x - i, startPos.y});
	}
}

void Snake::move(const SnakeInput& input)
{
	if(lastInput.IsOpossite(input) == false)
	{
		lastInput.x = input.x;
		lastInput.y = input.y;
	}

	m_links.RemoveNode(m_links.GetTail());
	m_links.InsertNode(m_links.GetHead()->GetValue(), m_links.GetHead()->GetNextNode());
	m_links.GetHead()->GetValue() += Position(lastInput.x, lastInput.y);
}