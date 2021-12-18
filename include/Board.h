#pragma once

#include <vector>
#include <memory>

#include "constants.h"
#include "Levels.h"
#include "pipes/Tile.h"
#include "pipes/TJunction.h"
#include "pipes/Turn.h"
#include "pipes/Straight.h"
#include "pipes/Cross.h"
#include "pipes/Sink.h"
#include "pipes/Tap.h"
#include "pipes/Empty.h"
#include "graph/Graph.h"

#include <SFML/Graphics.hpp>

class MovableObject;

class Board {
public:
	Board(sf::RenderWindow*);

	// maps
	void loadMaps(); // load all maps
	bool loadNextMap(); // build data straucture 
	int getCurrentLevel() const;
	void setCurrentLevel(int) ;

	// check if level is complete and load next map if any
	void checkBoardState();

	// display game
	void drawMap() const;
	void printStats() const;
	void drawLevelComplete() const;
	void drawWelcome() const;

	// the SFML window
	float getWindowHeight() const;
	float getWindowWidth() const;
	void resetWindowSize();

	// the game clock
	void resetGameClock();

	// game states
	bool gameIsOver() const;
	void setGameOver(bool b);
	bool levelCompleted() const;

	// user actions
	Tile* getTileClicked(int,int) const;
	void rotateRight(Tile*);
	void rotateLeft(Tile*);

	int getTapCount() const;

private:
	void resetGame();

	// all maps of the games
	Levels m_levels;

	sf::Clock m_gameClock; // curent level time
	sf::Clock m_animClock; // for smooth animation

	std::vector< std::vector<std::shared_ptr<Tile>>> m_tiles; // matrix of game board

	Graph<std::shared_ptr<Tile>> m_graph; //the flow graph using Tile objects in m_tiles
	
	std::vector<std::shared_ptr<Tile>> m_sinks; // list of sinks from m_tiles
	std::vector<std::shared_ptr<Tile>> m_taps; // list of taps from m_tiles

	// build the movable/unmovable data structures from the game map
	void loadMapObjects();

	// the graph for computing the flow (DFS)
	void buildGraph();
	void deleteNodeEdges(int row, int col); 
	void buildNodeEdges(int row, int col);
	void buildNodeEdgesTopLeft(int row, int col);
	void buildNodeEdgesRightBottom(int row, int col);

	// the drawing window
	sf::RenderWindow * m_window;

	bool m_gameIsOver = false;

	float m_xoffset = 0; // to center the board
	float m_yoffset = 0;

	int m_tapCount = 0;

};
