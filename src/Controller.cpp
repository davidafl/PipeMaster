#include "..\include\Controller.h"
#include <iostream>
#include "pipes/Tile.h"

Controller::Controller(): m_window(sf::RenderWindow(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Pipe Master by David")),
m_board(&m_window)
{
}

// The main function handling game states  and calling
// board.getTileClicked to rotate pipes
// States:
// Welcome: opening screen
// Playing: playing a level
// LevelCompleted: play sound upon level completion
// LevelComplete: display completed level screen
// GameOver: all levels completed, display game over message

void Controller::run()
{
    try {
        m_board.loadMaps();
        Resources::instance().playSound(1, LOOP, 50);

        while (m_window.isOpen())
        {
            m_window.clear();


            switch (m_gameMode) {
            case GameMode::Welcome:
                m_board.drawWelcome();
                // print welcome screen
                // tap anywhere to continue
                break;
            case GameMode::Playing:
                if (m_board.levelCompleted())
                    m_gameMode = GameMode::LevelCompleted;
                else
                    m_board.checkBoardState();

                m_board.drawMap();

                break;
            case GameMode::LevelCompleted:
                Resources::instance().playSound(WATER_SOUND, NO_LOOP,100);
                m_gameMode = GameMode::LevelComplete;
                break;

            case GameMode::LevelComplete:
                m_board.drawMap();
                m_board.drawLevelComplete();
                break;
            case GameMode::GameOver:
                m_board.drawMap();
                m_board.drawLevelComplete();
                break;
            }

            if (m_board.gameIsOver())
                m_gameMode = GameMode::GameOver;


            m_window.display();

            if (auto event = sf::Event{}; m_window.pollEvent(event))
            {
                switch (event.type)
                {
                case sf::Event::Closed:
                {
                    m_window.close();
                    break;
                }

                case sf::Event::MouseButtonPressed:
                {
                    switch (m_gameMode) {
                    case GameMode::Playing:
                    {
                        Tile* t = m_board.getTileClicked(event.mouseButton.x, event.mouseButton.y);
                        if (t != NULL) {
                            Resources::instance().playSound(CLICK_SOUND, NO_LOOP, 100);
                            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                                m_board.rotateLeft(t);
                            else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                                m_board.rotateRight(t);
                        }
                        break;
                    }
                    case GameMode::LevelComplete:
                    {
                        m_gameMode = GameMode::Playing;
                        m_board.checkBoardState();
                        break;
                    }
                    case  GameMode::Welcome:
                    {
                        m_gameMode = GameMode::Playing;
                        break;
                    }
                    case GameMode::GameOver:
                    {
                        return;
                        break;
                    }
                    default:
                        break;
                    }

                    break;
                }
                default: 
                    break;
                }
            }
        }
    }
    catch (std::runtime_error& e) {
        // exit - failure to load maps
        CLogger::GetLogger()->Log(e.what());
        CLogger::GetLogger()->Log("Failure to load game, exiting!");
    }
    
}

