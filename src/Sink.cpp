#include "pipes/Sink.h"


Sink::Sink(int row, int col, int x, int y, int orientation) : Tile(row, col, x, y, orientation)
{
	initFlowDirections(true, false, false, false);
	m_sprite.setTexture(Resources::instance().getImage(SINK*2));
}

int Sink::getImageIndex() const {
	return SINK*2;
}

void Sink::rotateLeft() {
}

void Sink :: rotateRight(){

}