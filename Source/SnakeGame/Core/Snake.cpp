#include "Core/Snake.h"

DEFINE_LOG_CATEGORY_STATIC(LogSnake, All, All);

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

void SnakeGame::Snake::printDebug()
{
#if !UE_BUILD_SHIPPING
	int32 i = 0;
	for(auto link : m_links)
	{
		if(i == 0)
		{
			UE_LOG(LogSnake, Display, TEXT("SnakeHeadPosition: (%i, %i)"), link.x, link.y);
		}
		else
		{
			UE_LOG(LogSnake, Display, TEXT("SnakeLinkPosition %i: (%i, %i)"), i, link.x, link.y);
		}
		++i;
	}
#endif
}

void Snake::move(const SnakeInput& input)
{
	if(lastInput.IsOpossite(input) == false)
	{
		lastInput.x = input.x;
		lastInput.y = input.y;
	}

	m_links.MoveTail(m_links.GetTail(), m_links.GetHead(), m_links.GetHead()->GetValue());
	m_links.GetHead()->GetValue() += Position(lastInput.x, lastInput.y);
}

void SnakeGame::Snake::increase()
{
	Position lastLinkValue = m_links.GetTail()->GetValue();
	Position preLastLinkValue = m_links.GetTail()->GetPrevNode()->GetValue();
	m_links.AddTail(lastLinkValue + (lastLinkValue - preLastLinkValue));
}