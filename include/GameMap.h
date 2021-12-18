#pragma once


#include <vector>

class GameMap {

public:

    GameMap(int h, int w);

    int getHeight() const;
    int getWidth() const;

    void setHeight(int);
    void setWidth(int);
    
    void resize(int size);

    std::pair<int, int> getTileInfo(int row, int col) const;
    void push(int row, std::pair<int, int>);

private:
    // a single map
    std::vector<std::vector<std::pair<int,int>>> m_tiles; 

    int m_height;
    int m_width;
};


