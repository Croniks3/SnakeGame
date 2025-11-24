#include "Core/Food.h"
#include "LoggingConfig.h"


DEFINE_LOG_CATEGORY_STATIC(LogFood, LOG_DEFAULT_VERBOSITY, LOG_COMPILETIME_VERBOSITY);

using namespace SnakeGame;


void Food::setPosition(const Position& position)
{
	m_position = position;
}

Position Food::getPosition() const
{
	return m_position;
}

#if !UE_BUILD_SHIPPING
void Food::printDebug()
{
	UE_LOG(LogFood, Display, TEXT("FoodPosition: (%i, %i)"), m_position.x, m_position.y);
}
#endif