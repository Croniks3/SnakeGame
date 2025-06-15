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

		void move(const SnakeInput& input);
		
	private:
		TSnakeList m_links;
	};
}