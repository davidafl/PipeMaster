#pragma once
#include <SFML/Graphics.hpp>
#include "pipes/Tile.h"
#include "Board.h"

class Controller {

public:

	Controller();
	void run();

    enum class GameMode
    {
        Welcome,
        Playing,
        LevelCompleted,
        LevelComplete,
        GameOver
    };

private:
    sf::RenderWindow m_window;
    Board m_board;

    GameMode m_gameMode = GameMode::Welcome;
};
