#include "pipes/Empty.h"


Empty::Empty(int row, int col, int x, int y, int orientation) : Tile(row, col, x, y, orientation)
{
	initFlowDirections(false, false, false, false);
	m_sprite.setTexture(Resources::instance().getImage(EMPTY*2));
}

int Empty::getImageIndex() const {
	return EMPTY*2;
}
