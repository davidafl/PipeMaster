#include "pipes/Tap.h"


Tap::Tap(int row, int col, int x, int y, int orientation) : Tile(row, col, x, y, orientation)
{
	initFlowDirections(true, false, false, false);
	m_sprite.setTexture(Resources::instance().getImage(TAP*2));
	m_flow = true;
}

int Tap::getImageIndex()const  {
	return TAP*2;
}
