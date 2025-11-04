#include "Core/Food.h"

using namespace SnakeGame;


void Food::setPosition(const Position& position)
{
	m_position = position;
}

Position Food::getPosition() const
{
	return m_position;
}