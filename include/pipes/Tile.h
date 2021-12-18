#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"

// the base class for pipe tiles
class Tile {

public:

	Tile(int row, int col, int x, int y, int orientation);
	virtual ~Tile() = default;

	virtual void rotateLeft();
	virtual void rotateRight();
	int getOrientation() const;

	void draw(sf::RenderWindow& window);
	bool clicked(int, int) const;

	void setFlow(bool b);
	bool getFlow() const;
	void initFlowDirections(bool, bool, bool, bool);
	bool isDirectionOpen(int dir) const;

	virtual int getImageIndex() const;

	int getRow() const;
	int getCol() const;

protected:
	sf::Sprite m_sprite;
	bool m_flow = false;

private:

	int m_x, m_y; // screen coordinates
	int m_row, m_col;
	int m_orientation; // up (0) right (1) down (2) left (3)
	int m_animDirection; // direction of animation

	sf::Clock m_clock; // for PNG animated

	std::vector<bool> m_directions;

	void rotateToDirection(int dir);
	void updateImage();
};
