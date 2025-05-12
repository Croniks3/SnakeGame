#pragma once

#include "CoreMinimal.h"
#include "SG_Types.h"


namespace Snake
{
	class Grid;

	class Game
	{
	public:
		Game(const Settings& settings);
		TSharedPtr<Grid> getGrid() const { return m_grid; };

	private:
		const Settings c_settings;
		TSharedPtr<Grid> m_grid;
	};
}