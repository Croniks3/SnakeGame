#include "Core/Game.h"
#include "Core/Grid.h"
#include "Core/Snake.h"
#include "Core/Food.h"

DEFINE_LOG_CATEGORY_STATIC(LogGrid, All, All);

//#pragma optimize("", off)

using namespace SnakeGame;


Game::Game(const Settings& settings) : c_settings(settings)
{
	m_grid = MakeShared<Grid>(settings.gridSize);
	m_snake = MakeShared<Snake>(settings.snakeSettings);
	m_food = MakeShared<Food>();

	updateGrid();
	generateFood();
}

void Game::update(float deltaSeconds, const SnakeInput& input)
{
	if(checkUpdatePossibility(deltaSeconds) == false || m_gameOver == true)
	{
		return;
	}

	moveSnake(input);

	if(isDied() == true)
	{
		m_gameOver = true;
		UE_LOG(LogGrid, Display, TEXT("------------- Game over! ------------- "));
	}
}

void Game::moveSnake(const SnakeInput& input)
{
	m_snake->move(input);
	updateGrid();
}

void Game::updateGrid()
{
	m_grid->update(m_snake->getBody(), CellType::SnakeCell);
	m_grid->printDebug();
}

bool SnakeGame::Game::checkUpdatePossibility(float deltaSeconds)
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

void SnakeGame::Game::generateFood()
{
	m_food->setPosition(Position{1, 1});
	m_grid->update(m_food->getPosition(), CellType::FoodCell);
}