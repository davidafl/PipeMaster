#include "pipes/Straight.h"


Straight::Straight(int row, int col, int x, int y, int orientation) : Tile(row, col, x, y, orientation)
{
	initFlowDirections(true, false, true, false);
	m_sprite.setTexture(Resources::instance().getImage(STRAIGHT*2));
}

int Straight::getImageIndex() const  {
	return STRAIGHT*2;
}
