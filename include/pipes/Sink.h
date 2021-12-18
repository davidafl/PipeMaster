#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"
#include "Tile.h"
#include "Resources.h"

class Sink : public Tile {
public:
	Sink(int row, int col, int x, int y, int orientation);
	int getImageIndex() const override;
	void rotateLeft() override;
	void rotateRight() override;
};