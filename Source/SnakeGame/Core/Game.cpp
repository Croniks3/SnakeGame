#include "Core/Game.h"
#include "Core/Grid.h"
#include "Core/Snake.h"
#include "Core/Food.h"
#include "LoggingConfig.h"

DEFINE_LOG_CATEGORY_STATIC(LogGame, LOG_DEFAULT_VERBOSITY, LOG_COMPILETIME_VERBOSITY);

//#pragma optimize("", off)

using namespace SnakeGame;


Game::Game(const Settings& settings) : c_settings(settings)
{
	m_grid = MakeShared<Grid>(settings.gridSize);

	checkf(m_grid->dimensions().width >= settings.snakeSettings.defaultSize, 
		TEXT("Snake initial length: %i, doesn't fit grid width: %i!"), 
		settings.snakeSettings.defaultSize,
		m_grid->dimensions().width
	);

	m_snake = MakeShared<Snake>(settings.snakeSettings);
	m_food = MakeShared<Food>();

	m_grid->update(m_snake->getBody(), CellType::SnakeCell);
	generateFood();
	m_grid->update(m_food->getPosition(), CellType::FoodCell);
}

void Game::update(float deltaSeconds, const SnakeInput& input)
{
	if(m_foodTaken == true)
	{
		m_gameplayEventCallback(GameplayEvent::FoodTaken);

		++m_scores;
		m_snake->increase();
		m_grid->update(m_snake->getBody(), CellType::SnakeCell);

		const bool foodWasGenerated = generateFood();
		if(foodWasGenerated == false)
		{
			m_gameplayEventCallback(GameplayEvent::GameCompleted);

			m_gameOver = true;
			return;
		}

		m_foodTaken = false;
	}

	if(checkUpdatePossibility(deltaSeconds) == false || m_gameOver == true)
	{
		return;
	}

	m_snake->move(input);

	if(isDied() == true)
	{
		m_gameplayEventCallback(GameplayEvent::GameOver);

		m_gameOver = true;
		return;
	}

	m_foodTaken = takeFood();
	updateGrid();
}

void Game::updateGrid()
{
	m_grid->update(m_snake->getBody(), CellType::SnakeCell);
	m_grid->update(m_food->getPosition(), CellType::FoodCell);

	//UE_LOG(LogGame, Display, TEXT("(Class = Game, Method = updateGrid()):-------------------------- "));

#if !UE_BUILD_SHIPPING
	/*m_grid->printDebug();
	m_food->printDebug();
	m_snake->printDebug();*/
#endif
}

bool Game::checkUpdatePossibility(float deltaSeconds)
{
	m_timeSinceLastUpdate += deltaSeconds;

	if(m_timeSinceLastUpdate >= c_settings.gameSpeed)
	{
		m_timeSinceLastUpdate = 0.0f;
		return true;
	}

	return false;
}

bool Game::isDied() const
{
	return m_grid->hitTest(m_snake->getHeadPosition(), CellType::WallCell) ||
		m_grid->hitTest(m_snake->getHeadPosition(), CellType::SnakeCell);
}

bool Game::takeFood() const
{
	return m_grid->hitTest(m_snake->getHeadPosition(), CellType::FoodCell);
}

bool Game::generateFood()
{
	Position foodRandomPos;
	if(m_grid->getRandomEmptyPosition(m_snake->getHeadPosition(), foodRandomPos) == true)
	{
		m_food->setPosition(foodRandomPos);
		return true;
	}
	
	return false;
}

void Game::subscribeOnGameplayEvent(GameplayEventCallback callback)
{
	m_gameplayEventCallback = callback;
}