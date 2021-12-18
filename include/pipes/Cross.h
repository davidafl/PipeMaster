#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"
#include "Tile.h"
#include "Resources.h"

class Cross : public Tile {
public:
	Cross(int row, int col, int x, int y, int orientation);
	int getImageIndex() const override;
};