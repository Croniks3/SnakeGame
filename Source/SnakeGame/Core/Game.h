#pragma once

#include <CoreMinimal.h>
#include "Core/SG_Types.h"


namespace SnakeGame
{
	class Grid;
	class Snake;
	class Food;

	
	class Game
	{
	public:
		Game(const Settings& settings);
		TSharedPtr<Grid> getGrid() const { return m_grid; };
		TSharedPtr<Snake> getSnake() const { return m_snake; };
		TSharedPtr<Food> getFood() const { return m_food; };

		void update(float deltaSeconds, const SnakeInput& input);

	private:
		const Settings c_settings;
		TSharedPtr<Grid> m_grid;
		TSharedPtr<Snake> m_snake;
		TSharedPtr<Food> m_food;

		float m_timeSinceLastUpdate{0.0f};
		bool m_gameOver{false};
		bool m_foodTaken{false};
		uint32 m_scores{0};
		
		void updateGrid();
		bool checkUpdatePossibility(float deltaSeconds);
		bool isDied() const;
		bool takeFood() const;
		bool generateFood();
	};
}