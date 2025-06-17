#pragma once

#include <CoreMinimal.h>
#include "Core/SG_Types.h"


namespace SnakeGame
{
	class Grid;
	class Snake;
	
	class Game
	{
	public:
		Game(const Settings& settings);
		TSharedPtr<Grid> getGrid() const { return m_grid; };
		TSharedPtr<Snake> getSnake() const { return m_snake; };

		void update(float deltaSeconds, const SnakeInput& input);

	private:
		const Settings c_settings;
		TSharedPtr<Grid> m_grid;
		TSharedPtr<Snake> m_snake;
		float m_timeSinceLastUpdate{0.0f};
		bool m_gameOver{false};

		void moveSnake(const SnakeInput& input);
		void updateGrid();
		bool checkUpdatePossibility(float deltaSeconds);
		bool isDied() const;
	};
}