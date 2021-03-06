#include "Resources.h"
#include <iostream>

const int NUM_IMAGES = 18;

const char* game_images[NUM_IMAGES] = { "tjunction.png", "tjunction_full.png", 
                                          "cross.png",  "cross_full.png",
                                                "straight.png",  "straight_full.png",
                                        "turn.png", "turn_full.png",
                                              "tap.png",  "tap_full.png",
                                                "sink.png",  "sink_full.png",
                                              "empty.png",  "empty_full.png",
                                            "background.png", "solved.png", "gameover.png", "splash.png" };

const int NUM_SOUNDS = 3;
const char* sounds[] = { "water.wav","background.wav", "click.wav" };


/*
*  A SINGLETON class to load game images, sound and font
*/


Resources::~Resources()
{
    m_textures.clear();
    m_soundsBuffer.clear();
    m_sounds.clear();
}

Resources& Resources::instance()
{
    static Resources inst;
    return inst;
}

sf::Texture& Resources::getImage(int index) 
{
    return m_textures[index];
}


void Resources::playSound(int type, bool loop, int vol)
{
    if (loop)
        m_sounds[type].setLoop(true);
    else
        m_sounds[type].setLoop(false);

    m_sounds[type].setVolume((float) vol);
    m_sounds[type].play();
}

sf::Font& Resources::getFont()
{
    return m_gameFont;
}

Resources::Resources()
{

    m_textures.resize(NUM_IMAGES);

    for (int i = 0; i < NUM_IMAGES; i++) {
        if (!m_textures[i].loadFromFile(game_images[i])) {
            const std::string err = "failed to load image file:" + std::string(game_images[i]);
            throw std::runtime_error(err);
        }    
    }

    // load font
    if (!m_gameFont.loadFromFile("SHOWG.TTF"))
    {
        throw std::runtime_error("failed to load font file");
    }


    // load sound.
    m_soundsBuffer.resize(NUM_SOUNDS);
    m_sounds.resize(NUM_SOUNDS);
    for (int i = 0; i < NUM_SOUNDS; i++) {
        if (!m_soundsBuffer[i].loadFromFile(sounds[i])) {
            const std::string err = "failed to load sound file " +std::string(sounds[i]);
            throw std::runtime_error(err);
        }
        else
            m_sounds[i].setBuffer(m_soundsBuffer[i]);
    }
}



