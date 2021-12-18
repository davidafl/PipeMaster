
#include "Levels.h"
using namespace std;


// load the maps from file - called only once
void Levels::loadMapsFromFile(const char* file_name) {

    ifstream if1;
    if1.open(file_name);
    int linenumber = 0;

    if (!if1) {
        throw std::runtime_error(OPEN_FILE_ERR);
    }

    int tile_type, tile_rotation;
    int width, height;
    int mapindex = 0;
    bool mapcomplete = false; // we read a whole map

    try {
        
        while (true) {

            std::istringstream line = readLine(if1);
            
            if (line.eof() || (line >> std::ws).eof()) {
                // empty line means EOF
                break;
            }

            linenumber++;

            line >> skipws >> height;
            line >> skipws >> width;

            if (!(line.eof() || (line >> std::ws).eof())) {
                throw std::runtime_error(EXTRA_DATA_ERR + std::to_string(linenumber));
            }

            // add the map and set its size
            m_maps.push_back(GameMap(height, width));

            m_maps[mapindex].resize(height);

            // read the map
            mapcomplete = false;
            for (int row = 0; row < height; row++) {

                line = readLine(if1);
                linenumber++;

                for (int col = 0; col < width; col++) {
                    line >> tile_type;
                    if (tile_type < 0 || tile_type > MAX_TILE_TYPE) {
                        throw std::runtime_error(MISSING_DATA_ERR + std::to_string(linenumber));
                    }

                    line >> tile_rotation;
                    if (tile_rotation < 0 || tile_rotation > MAX_DIRECTION) {
                        throw std::runtime_error(MISSING_DATA_ERR + std::to_string(linenumber));
                    }

                    std::pair<int, int> tile = std::make_pair(tile_type, tile_rotation);
                    m_maps[mapindex].push(row, tile);
                }
                if (!(line.eof() || (line >> std::ws).eof())) {
                    throw std::runtime_error(EXTRA_DATA_ERR + std::to_string(linenumber));
                }
            }
            mapcomplete = true;
            mapindex++;
        }

        if1.close();

    }
    catch (std::ios_base::failure& )
    {
        throw std::runtime_error(MISSING_DATA_ERR + std::to_string(linenumber));
    }
    catch (std::runtime_error& e)
    {
        throw e;
    }
    catch (std::exception& )
    {
        if (!mapcomplete)
            throw std::runtime_error(INCOMPLETE_ERR + std::to_string(linenumber));
    }

}

std::istringstream Levels::readLine(ifstream& m_istr) {
    auto line = std::string();
    m_istr.clear();

    if (std::getline(m_istr, line)) {
        // store our current line
       auto curLine = std::istringstream(line);

        // configure stream to throw exception upon invalid data
        curLine.exceptions(std::ios::failbit | std::ios::badbit);

        return curLine;
    }
    else {
        // return empty string
        auto line = std::istringstream("");
        line.exceptions(std::ios::failbit | std::ios::badbit);
        return line;
    }
}

int Levels::getCurrentLevel() const
{
    return m_current_level;
}

void Levels::setCurrentLevel(int lev)
{
    if (lev <0 || lev > m_maps.size())
        lev = 0;
    m_current_level = lev;
}

bool Levels::loadNextMap()
{
    //Resources::instance().playSound(PASS_SOUND);

    if (m_current_level < m_maps.size()-1) {
        m_current_level++;
        return true;
    }
    return false;
}

GameMap Levels::getCurrentMap() const
{
    return m_maps[m_current_level];
}

float Levels::getWindowHeight() const
{
    return (float) m_maps[m_current_level].getHeight()* TILE_HEIGHT;
}

float Levels::getWindowWidth() const
{
    return (float) m_maps[m_current_level].getWidth() * TILE_WIDTH;
}
