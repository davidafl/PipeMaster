#include "Board.h"

#include <string>

const char* LEVELS_FILE = "levels.txt";

/*
* param: the window game passed by the controller
*/
Board::Board(sf::RenderWindow* w):m_tiles(0, std::vector<std::shared_ptr<Tile>>(0))
{
    m_window = w;
}

void Board::loadMaps() 
{
    m_levels.loadMapsFromFile(LEVELS_FILE);
    setCurrentLevel(0);
    loadMapObjects();
}

/*
* loop over all objects  and draw them.
* at the end print game stats at the bottom of window
*/
void Board::drawMap() const
{
    sf::Sprite s;
    s.setTexture(Resources::instance().getImage(BG_IMAGE));
    s.setScale(getWindowWidth() / s.getLocalBounds().width, getWindowHeight() / s.getLocalBounds().height);
    m_window->draw(s);

    for (auto& row : m_tiles) {
        for (auto& col : row) {
            col ->draw(*m_window);
        }
    }

    printStats();
}


float Board::getWindowHeight() const
{
    auto h = m_levels.getCurrentMap().getHeight() * TILE_WIDTH + STATUS_HEIGHT + 2*WIN_BORDER_HEIGHT;
    if (h < WIN_HEIGHT)
        return WIN_HEIGHT;
    return (float) h;
}

float Board::getWindowWidth() const
{
    auto w = m_levels.getCurrentMap().getWidth() * TILE_WIDTH + 2*WIN_BORDER_WIDTH ;
    if (w < WIN_WIDTH)
        return WIN_WIDTH;
    return (float) w;
}

void Board::rotateRight(Tile* t) {
    m_tapCount++;
    // delete connected neighbors
    deleteNodeEdges(t->getRow(), t->getCol());

    t->rotateRight();

    // rebuild neightbors
    buildNodeEdges(t->getRow(), t->getCol());

    // reset flow boolean to false
    for (int row = 0; row < m_levels.getCurrentMap().getHeight(); row++)
        for (int col = 0; col < m_levels.getCurrentMap().getWidth(); col++)
            m_tiles[row][col].get()->setFlow(false);

    // run DFS and set flow status for dfs result
    std::vector<std::shared_ptr<Tile>> flows = m_graph.dfs();

    // update sprite/flow status
    for (auto& ti : flows)
        ti.get()->setFlow(true);

}

void Board::rotateLeft(Tile* t) {
    m_tapCount++;
    // delete connected neighbors
    deleteNodeEdges(t->getRow(), t->getCol());

    t->rotateLeft();

    // rebuild neightbors
    buildNodeEdges(t->getRow(), t->getCol());

    // reset flow boolean to false
    for (int row = 0; row < m_levels.getCurrentMap().getHeight(); row++)
        for (int col = 0; col < m_levels.getCurrentMap().getWidth(); col++)
            m_tiles[row][col].get()->setFlow(false);

    // run DFS and set flow status for dfs result
    std::vector<std::shared_ptr<Tile>> flows = m_graph.dfs();

    // update sprite/flow status
    for (auto& ti : flows)
        ti.get()->setFlow(true);
    
    //buildGraph();
}

void Board::resetGame() {
    m_tapCount = 0;
    m_sinks.clear();
    m_taps.clear();
    m_tiles.clear();
    m_gameIsOver = false;
}

int Board::getTapCount() const
{
    return m_tapCount;
}


void Board::loadMapObjects()
{
    resetGame();

    GameMap curmap = m_levels.getCurrentMap();
    resetWindowSize();

    m_xoffset = (getWindowWidth() - curmap.getWidth() * TILE_WIDTH)/2;
    m_yoffset = (getWindowHeight() - curmap.getHeight() * TILE_HEIGHT) / 2;
    

    for (int row = 0; row < curmap.getHeight(); row++) {

        m_tiles.push_back(std::vector<std::shared_ptr<Tile>>());

        for (int col = 0; col < curmap.getWidth(); col++) {

            switch (curmap.getTileInfo(row, col).first) {
            case TJUNCTION: //Tile::BlockType::TJunction:

                m_tiles[row].push_back(std::make_shared<TJunction>(
                    row, col,
                    col * TILE_WIDTH + m_xoffset + TILE_WIDTH / 2, // half size because rotation
                    row * TILE_HEIGHT + m_yoffset + TILE_HEIGHT / 2,
                    curmap.getTileInfo(row, col).second));
                break;

            case CROSS:
                m_tiles[row].push_back(std::make_shared<Cross>(
                    row, col,
                    col * TILE_WIDTH + m_xoffset + TILE_WIDTH / 2, // half size because rotation
                    row * TILE_HEIGHT + m_yoffset + TILE_HEIGHT / 2,
                    curmap.getTileInfo(row, col).second));
                break;

            case STRAIGHT:

                m_tiles[row].push_back(std::make_shared<Straight>(
                    row, col,
                    col * TILE_WIDTH + m_xoffset + TILE_WIDTH / 2, 
                    row * TILE_HEIGHT + m_yoffset + TILE_HEIGHT / 2,
                    curmap.getTileInfo(row, col).second));
                break;

            case TURN: 
                m_tiles[row].push_back(std::make_shared<Turn>(
                    row, col,
                    col * TILE_WIDTH + m_xoffset + TILE_WIDTH / 2, // half size because rotation
                    row * TILE_HEIGHT + m_yoffset + TILE_HEIGHT / 2,
                    curmap.getTileInfo(row, col).second));

                break;

            case TAP:
                m_tiles[row].push_back(std::make_shared<Tap>(
                    row, col,
                    col * TILE_WIDTH + m_xoffset + TILE_WIDTH / 2, // half size because rotation
                    row * TILE_HEIGHT + m_yoffset + TILE_HEIGHT / 2,
                    curmap.getTileInfo(row, col).second));

                m_taps.push_back(m_tiles[row][col]);

                break;

            case SINK:
                m_tiles[row].push_back(std::make_shared<Sink>(
                    row, col,
                    col * TILE_WIDTH + m_xoffset + TILE_WIDTH / 2, // half size because rotation
                    row * TILE_HEIGHT + m_yoffset + TILE_HEIGHT / 2,
                    curmap.getTileInfo(row, col).second));

                m_sinks.push_back(m_tiles[row][col]);

                break;

            case EMPTY:
                m_tiles[row].push_back(std::make_shared<Empty>(
                    row, col,
                    col * TILE_WIDTH + m_xoffset + TILE_WIDTH / 2, // half size because rotation
                    row * TILE_HEIGHT + m_yoffset + TILE_HEIGHT / 2,
                    curmap.getTileInfo(row, col).second));
                break;

            default:
                break;
            }
        }
    }

    buildGraph();
    resetGameClock();
}


// delete all to and from the Tile at )row,col)
void Board::deleteNodeEdges(int row, int col) {
    auto t = m_tiles[row][col];

    // left
    if (col > 0) {
        if (t.get()->isDirectionOpen(DIR_LEFT) && m_tiles[row][col - 1].get()->isDirectionOpen(DIR_RIGHT)) {
            m_graph.removeEdge(t, m_tiles[row][col - 1]);
            m_graph.removeEdge(m_tiles[row][col - 1], t);
        }
    }
    // up
    if (row > 0) {
        if (t.get()->isDirectionOpen(DIR_UP) && m_tiles[row - 1][col].get()->isDirectionOpen(DIR_DOWN)) {
            m_graph.removeEdge(t, m_tiles[row - 1][col]);
            m_graph.removeEdge(m_tiles[row - 1][col], t);
        }
    }
    // right
    if (col < m_levels.getCurrentMap().getWidth() - 1) {
        if (t.get()->isDirectionOpen(DIR_RIGHT) && m_tiles[row][col + 1].get()->isDirectionOpen(DIR_LEFT)) {
            m_graph.removeEdge(t, m_tiles[row][col + 1]);
            m_graph.removeEdge(m_tiles[row][col + 1], t);
        }
    }
    // bottom 
    if (row < m_levels.getCurrentMap().getHeight() - 1) {
        if (t.get()->isDirectionOpen(DIR_DOWN) && m_tiles[row + 1][col].get()->isDirectionOpen(DIR_UP)) {
            m_graph.removeEdge(t, m_tiles[row + 1][col]);
            m_graph.removeEdge(m_tiles[row + 1][col], t);
        }
    }
}

// build top left right bottom connected edges
void Board::buildNodeEdges(int row, int col) {

    buildNodeEdgesTopLeft(row, col);
    buildNodeEdgesRightBottom(row, col);
}

void Board::buildNodeEdgesTopLeft(int row, int col) {
    auto t = m_tiles[row][col];

    // left
    if (col > 0) {
        if (t.get()->isDirectionOpen(DIR_LEFT) && m_tiles[row][col - 1].get()->isDirectionOpen(DIR_RIGHT)) {
            m_graph.addEdge(t, m_tiles[row][col - 1]);
            m_graph.addEdge(m_tiles[row][col - 1], t);
        }
    }
    // up
    if (row > 0) {
        if (t.get()->isDirectionOpen(DIR_UP) && m_tiles[row - 1][col].get()->isDirectionOpen(DIR_DOWN)) {
            m_graph.addEdge(t, m_tiles[row - 1][col]);
            m_graph.addEdge(m_tiles[row - 1][col], t);
        }
    }
}

void Board::buildNodeEdgesRightBottom(int row, int col) {
    auto t = m_tiles[row][col];

    // right
    if (col < m_levels.getCurrentMap().getWidth() - 1) {
        if (t.get()->isDirectionOpen(DIR_RIGHT) && m_tiles[row][col + 1].get()->isDirectionOpen(DIR_LEFT)) {
            m_graph.addEdge(t, m_tiles[row][col + 1]);
            m_graph.addEdge(m_tiles[row][col + 1], t);
        }
    }
    // bottom 
    if (row < m_levels.getCurrentMap().getHeight() - 1) {
        if (t.get()->isDirectionOpen(DIR_DOWN) && m_tiles[row + 1][col].get()->isDirectionOpen(DIR_UP)) {
            m_graph.addEdge(t, m_tiles[row + 1][col]);
            m_graph.addEdge(m_tiles[row + 1][col], t);
        }
    }
}

void Board::buildGraph() {
    GameMap curmap = m_levels.getCurrentMap();

    // reset flow boolean to false
    for (int row = 0; row < curmap.getHeight(); row++)
        for (int col = 0; col < curmap.getWidth(); col++)
            m_tiles[row][col].get()->setFlow(false);

    m_graph.clear();

    // add vertices to graph
    for (int row = 0; row < curmap.getHeight(); row++) {
        for (int col = 0; col < curmap.getWidth(); col++) {

            auto t = m_tiles[row][col];
            m_graph.addVertex(t, curmap.getTileInfo(row, col).first == TAP);
        }
    }
    // build edges
    for (int row = 0; row < curmap.getHeight(); row++) {
        for (int col = 0; col < curmap.getWidth(); col++) {
            buildNodeEdgesTopLeft(row,col);
        }
    }
    // run DFS and set flow status for dfs result
    std::vector<std::shared_ptr<Tile>> flows = m_graph.dfs();

    // update sprite/flow status
    for (auto& t : flows) 
        t.get()->setFlow(true);

}


void Board::resetGameClock()
{
    m_gameClock.restart();
}



int Board::getCurrentLevel() const
{
    return m_levels.getCurrentLevel();
}

void Board::setCurrentLevel(int lev)
{
    m_levels.setCurrentLevel(lev);
}

/*
* load the next map available
* returns false if no more maps, else true
*/
bool Board::loadNextMap() {

    if (m_levels.loadNextMap()) {

        loadMapObjects();
        return true;
    }
    else
        return false; // no more maps game is over
}



/*
* check if level is complete and load next map if any
*/
void Board::checkBoardState() {

    // check if all sinks are full
    if (levelCompleted()) {
        if (!loadNextMap())
            setGameOver(true);
    }

}

// check if all sinks are full
bool Board::levelCompleted() const {
    if (m_sinks.size() == 0)
        return false;

    bool all_sinks = true;
    for (auto& s : m_sinks) {
        if (!s->getFlow())
            all_sinks = false;
    }
    
    return all_sinks;
}

void Board::drawLevelComplete() const
{
    sf::Sprite s;

    if (m_gameIsOver) {
        s.setTexture(Resources::instance().getImage(GAMEOVER_IMAGE));
    }
    else {
        s.setTexture(Resources::instance().getImage(SOLVED_IMAGE));
    }
    s.setPosition((getWindowWidth() - s.getLocalBounds().width) / 2, (getWindowHeight() - s.getLocalBounds().height) / 2);

    m_window->draw(s);

}

void Board::drawWelcome() const
{
    sf::Sprite s;

    s.setTexture(Resources::instance().getImage(WELCOME_IMAGE));
    //s.setPosition((getWindowWidth() - s.getLocalBounds().width) / 2, (getWindowHeight() - s.getLocalBounds().height) / 2);
    s.setScale(getWindowWidth() / s.getLocalBounds().width, getWindowHeight() / s.getLocalBounds().height);
    s.setPosition(0,0);

    m_window->draw(s);
}


bool Board::gameIsOver() const
{
    return m_gameIsOver;
}

void Board::setGameOver(bool b)
{
    m_gameIsOver = b;
}

Tile* Board::getTileClicked(int x, int y) const
{
    for (auto& row : m_tiles) {
        for (auto& col : row) {
            if (col->clicked(x, y))
                return col.get();
        }
    }
    return NULL;
}



/*
* prints the stats below the game board using sf::
*/
void Board::printStats() const {

    std::string score = 
          "Tap Count: "    + std::to_string(getTapCount()) 
        + "   |   Level: "    + std::to_string(getCurrentLevel()+1);


     score += "   |   Time: " 
            + std::to_string(int ( m_gameClock.getElapsedTime().asSeconds()));


    sf::Text text;
    text.setFont (Resources::instance().getFont()); 
    text.setStyle(sf::Text::Bold);
    text.setString(score);
    text.setFillColor(sf::Color::Blue);
    text.setOutlineThickness(2);
    text.setOutlineColor(sf::Color::White);
    text.setCharacterSize(50);

    sf::Rect bounds = text.getGlobalBounds();
    bounds.top = getWindowHeight() - STATUS_HEIGHT ;
    bounds.left = (getWindowWidth() - bounds.width) / 2;
    text.setPosition(bounds.left, bounds.top);

    m_window->draw(text);
}

void Board::resetWindowSize() {
    m_window->setView(sf::View(sf::FloatRect(0, 0, getWindowWidth(), getWindowHeight() )));
    m_window->setSize(sf::Vector2u((unsigned int) getWindowWidth(), (unsigned int) getWindowHeight() ));
}