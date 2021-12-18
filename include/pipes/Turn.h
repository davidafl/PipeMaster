#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"
#include "Tile.h"
#include "Resources.h"

class Turn : public Tile {
public:
	Turn(int row, int col, int x, int y, int orientation);
	int getImageIndex() const override;
};