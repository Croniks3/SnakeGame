#include "Core/Game.h"
#include "Core/Grid.h"
#include "Core/Snake.h"

DEFINE_LOG_CATEGORY_STATIC(LogGrid, All, All);

using namespace SnakeGame;


Game::Game(const Settings& settings) : c_settings(settings)
{
	m_grid = MakeShared<Grid>(settings.gridSize);
	m_snake = MakeShared<Snake>(settings.snakeSettings);

	updateGrid();
}

void Game::update(float deltaSeconds, const SnakeInput& input)
{
	moveSnake(input);
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