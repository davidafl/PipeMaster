#include "pipes/Turn.h"


Turn::Turn(int row, int col, int x, int y, int orientation) : Tile(row, col, x, y, orientation)
{
	initFlowDirections(true, true, false, false);
	m_sprite.setTexture(Resources::instance().getImage(TURN*2));
}

int Turn::getImageIndex() const {
	return TURN*2;
}
