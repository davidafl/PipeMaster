

#include "GameMap.h"

GameMap::GameMap(int h, int w) : m_width(w), m_height(h)
{
}


int GameMap::getHeight() const
{
	return m_height;
}

int GameMap::getWidth() const
{
	return m_width;
}


void GameMap::setHeight(int h)
{
	m_height = h;
}

void GameMap::setWidth(int w)
{
	m_width = w;
}


void GameMap::resize(int size)
{
	m_tiles.resize(size);
}


std::pair<int,int> GameMap::getTileInfo(int row, int col) const
{
	return m_tiles[row][col];
}

void GameMap::push(int row, std::pair<int,int> tile)
{
	m_tiles[row].push_back(tile);
}
