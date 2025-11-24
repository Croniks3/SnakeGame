#pragma once

#include "CoreMinimal.h"
#include "Core/SG_Types.h"


namespace SnakeGame
{
	class Snake
	{
	public:
		Snake(const SnakeSettings& snakeSettings);

		const TSnakeList& getLinks() const { return m_links; }
		const Position getHeadPosition() const { return m_links.GetHead()->GetValue(); }
		const TSnakeListNode* getBody() const { return m_links.GetHead()->GetNextNode(); }

		void printDebug();

		void move(const SnakeInput& input);
		void increase();
		
	private:
		TSnakeList m_links;
		SnakeInput m_lastInput{SnakeGame::SnakeInput::Default};
		Position m_previousTailPos{Position::Zero};
	};
}