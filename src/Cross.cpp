#include "pipes/Cross.h"


Cross::Cross(int row, int col, int x, int y, int orientation) : Tile(row, col, x, y, orientation)
{
	initFlowDirections(true, true, true, true);
	m_sprite.setTexture(Resources::instance().getImage(CROSS*2));
}

int Cross::getImageIndex() const{
	return CROSS*2;
}
