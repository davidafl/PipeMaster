#include "Resources.h"
#include "pipes/Tile.h"
#include <iostream>

Tile::Tile(int row, int col, int x, int y,  int orientation): m_row(row), m_col(col), m_x(x), m_y(y), m_orientation(orientation),m_directions(4, false)
{
    m_sprite.setTexture(Resources::instance().getImage(getImageIndex()));
    updateImage();
    m_sprite.setPosition((float) x , (float) y );
    m_animDirection = DIR_RIGHT; // loading animation

    // center of rotation
    m_sprite.setOrigin({ std::round(m_sprite.getLocalBounds().width / 2.f), std::round(m_sprite.getLocalBounds().height / 2.f) });
}

int Tile::getOrientation() const {
    return m_orientation;
}

void Tile::rotateLeft()
{
    rotateToDirection(DIR_LEFT);
}

void Tile::rotateRight()
{
    rotateToDirection(DIR_RIGHT);
}

void Tile::rotateToDirection(int dir) {

    if (dir == DIR_LEFT) {
        // rotate vertor of directions
        std::rotate(m_directions.begin(), m_directions.begin() + 1, m_directions.end());
        m_orientation--;
        if (m_orientation < 0)
            m_orientation = 3;
        m_animDirection = DIR_LEFT;
    }
    else {
        // rotate vector of directions
        std::rotate(m_directions.rbegin(), m_directions.rbegin() + 1, m_directions.rend());
        //rotate image
        m_orientation++;
        m_orientation = m_orientation % 4;
        m_animDirection = DIR_RIGHT;
    }

    m_clock.restart();

    // update sprite
    updateImage();
}

bool Tile::clicked(int x, int y) const {
    return m_sprite.getGlobalBounds().contains(sf::Vector2f((float) x, (float) y));
}

void Tile::setFlow(bool b)
{
    m_flow = b;
    updateImage();
}

bool Tile::getFlow() const
{
    return m_flow;
}

void Tile::initFlowDirections(bool up, bool right, bool down, bool left)
{
    m_directions[DIR_UP] = up;
    m_directions[DIR_RIGHT] = right;
    m_directions[DIR_DOWN] = down;
    m_directions[DIR_LEFT] = left;
    if (m_orientation > 0)
        std::rotate(m_directions.rbegin(), m_directions.rbegin() + m_orientation, m_directions.rend());
}

// set the image and rotate if needed
// assume the water image is right at consecutive index + 1
void Tile::updateImage()
{
    if (m_flow) // regular png
        m_sprite.setTexture(Resources::instance().getImage(getImageIndex()+1));
    else // full water png
        m_sprite.setTexture(Resources::instance().getImage(getImageIndex()));

    m_sprite.setRotation(m_orientation * 90.f);
}

void Tile::draw(sf::RenderWindow& window)
{
    float angle = 0;

    // animate rotations
    if (m_animDirection != DIR_NONE && m_clock.getElapsedTime().asSeconds() < ANIMATION_SPEED) {

        if (m_animDirection == DIR_LEFT)
            angle = - (90.f - (m_clock.getElapsedTime().asSeconds() * 90.f / ANIMATION_SPEED));
        else 
            if (m_animDirection == DIR_RIGHT)
                angle = 90.f - (m_clock.getElapsedTime().asSeconds() * 90.f / ANIMATION_SPEED);

        m_sprite.setRotation((m_orientation * 90.f) -  angle);
    }
    else {
        // no animation needed
        m_animDirection = DIR_NONE;
        m_sprite.setRotation(m_orientation * 90.f);
    }

	window.draw(m_sprite);
}


int Tile::getRow() const
{
    return m_row;
}

int Tile::getCol() const
{
    return m_col;
}

bool Tile::isDirectionOpen(int dir) const {
    return m_directions[dir];
}

int Tile::getImageIndex() const
{
    return 0;
}





