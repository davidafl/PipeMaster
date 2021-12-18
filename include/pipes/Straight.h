#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"
#include "Tile.h"
#include "Resources.h"

class Straight : public Tile {
public:
	Straight(int row, int col, int x, int y, int orientation);
	int getImageIndex() const override;
};