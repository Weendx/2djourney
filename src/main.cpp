#include <cstdint>
#include <iostream>
#include "core.h"
#include "player.h"
#include "text.h"

#include "objects/fpsCounter.h"

// marks:
// https://itecnote.com/tecnote/c-shapes-proportionally-resize-with-window-in-sfml-2-x/

int main(int argc, char* argv[]) {
    Core core;
    
    sf::Texture playerSheet;
    if (!playerSheet.loadFromFile("resources/drawable/playerSheet.png"))
        throw std::runtime_error("Can't load player sprites");
    Player* playerIdle1 = new Player(playerSheet, sf::IntRect(0, 0, 50, 37));
    core.registerObject(playerIdle1);

    sf::Font defaultFont;
    if (!defaultFont.loadFromFile("resources/fonts/default.ttf"))
        throw std::runtime_error("Can't load default font");
    FpsCounter* fpsCounter = new FpsCounter(defaultFont);
    core.registerObject(fpsCounter);

    core.setScale(1.5, 1.5);

    core.process();
    delete fpsCounter;
    delete playerIdle1;
}
