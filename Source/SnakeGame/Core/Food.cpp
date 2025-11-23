#include "Core/Food.h"

DEFINE_LOG_CATEGORY_STATIC(LogFood, All, All);

using namespace SnakeGame;


void Food::setPosition(const Position& position)
{
	m_position = position;
}

Position Food::getPosition() const
{
	return m_position;
}

void Food::printDebug()
{
#if !UE_BUILD_SHIPPING
	UE_LOG(LogFood, Display, TEXT("FoodPosition: (%i, %i)"), m_position.x, m_position.y);
#endif
}