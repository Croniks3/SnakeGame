#include "Core/Snake.h"
#include "LoggingConfig.h"

DEFINE_LOG_CATEGORY_STATIC(LogSnake, LOG_DEFAULT_VERBOSITY, LOG_COMPILETIME_VERBOSITY);

using namespace SnakeGame;


Snake::Snake(const SnakeSettings& snakeSettings)
{
	checkf(snakeSettings.defaultSize >= 2, TEXT("Snake length is to small: %i"), snakeSettings.defaultSize);

	// Add snake links horizontaly to the lef [ ---* ]
	const auto startPos = snakeSettings.startPosition;
	for(uint32 i = 0; i < snakeSettings.defaultSize; ++i)
	{
		m_links.AddTail(Position{startPos.x - i, startPos.y});
	}

	// remember the position of the tail
	Position lastLinkValue = m_links.GetTail()->GetValue();
	Position preLastLinkValue = m_links.GetTail()->GetPrevNode()->GetValue();
	m_previousTailPos = lastLinkValue + (lastLinkValue - preLastLinkValue);
}

#if !UE_BUILD_SHIPPING
void SnakeGame::Snake::printDebug()
{
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
}
#endif

void Snake::move(const SnakeInput& input)
{
	m_previousTailPos = m_links.GetTail()->GetValue();

	if(m_lastInput.IsOpossite(input) == false)
	{
		m_lastInput.x = input.x;
		m_lastInput.y = input.y;
	}
	
	m_links.GetTail()->GetValue() = m_links.GetHead()->GetValue();
	m_links.MoveTailAfterHead();
	m_links.GetHead()->GetValue() += Position(m_lastInput.x, m_lastInput.y);
}

void Snake::increase()
{
	m_links.AddTail(m_previousTailPos);
}