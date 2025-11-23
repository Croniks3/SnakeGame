#include "Core/Game.h"
#include "Core/Grid.h"
#include "Core/Snake.h"
#include "Core/Food.h"

DEFINE_LOG_CATEGORY_STATIC(LogGame, All, All);

//#pragma optimize("", off)

using namespace SnakeGame;


Game::Game(const Settings& settings) : c_settings(settings)
{
	m_grid = MakeShared<Grid>(settings.gridSize);
	m_snake = MakeShared<Snake>(settings.snakeSettings);
	m_food = MakeShared<Food>();

	m_grid->update(m_snake->getBody(), CellType::SnakeCell);
	generateFood();
	m_grid->update(m_food->getPosition(), CellType::FoodCell);
}

void Game::update(float deltaSeconds, const SnakeInput& input)
{
	if(checkUpdatePossibility(deltaSeconds) == false || m_gameOver == true)
	{
		return;
	}

	m_snake->move(input);

	if(m_foodTaken == true)
	{
		++m_scores;
		m_snake->increase();
		m_grid->update(m_snake->getBody(), CellType::SnakeCell);
		generateFood();
	}

	if(isDied() == true)
	{
		m_gameOver = true;
		UE_LOG(LogGame, Display, TEXT("(Class = Game, Method = updateGrid()): ------------- Game over! ------------- "));
		UE_LOG(LogGame, Display, TEXT("(Class = Game, Method = updateGrid()): ------------- SCORES: %i! ------------- "), m_scores);
		return;
	}

	m_foodTaken = takeFood();
	updateGrid();
}

void Game::updateGrid()
{
	m_grid->update(m_snake->getBody(), CellType::SnakeCell);
	m_grid->update(m_food->getPosition(), CellType::FoodCell);

	UE_LOG(LogGame, Display, TEXT("(Class = Game, Method = updateGrid()):-------------------------- "));

	m_grid->printDebug();
	m_snake->printDebug();
	m_food->printDebug();
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

bool SnakeGame::Game::takeFood() const
{
	return m_grid->hitTest(m_snake->getHeadPosition(), CellType::FoodCell);
}

void SnakeGame::Game::generateFood()
{
	m_food->setPosition(m_grid->getRandomEmptyPosition(m_snake->getHeadPosition()));
}