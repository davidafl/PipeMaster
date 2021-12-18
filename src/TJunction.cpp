#include "pipes/TJunction.h"


TJunction::TJunction(int row, int col, int x, int y, int orientation) : Tile (row,col,x,y, orientation)
{
	initFlowDirections(true, true, true, false);
	m_sprite.setTexture(Resources::instance().getImage(TJUNCTION*2));
}

int TJunction::getImageIndex() const {
	return TJUNCTION*2;
}
