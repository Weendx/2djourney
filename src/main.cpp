#include <cstdint>
#include <iostream>
#include "core.h"
#include "objects/player.h"
#include "text.h"
#include "actor.h"
#include "objects/fpsCounter.h"

// marks:
// https://itecnote.com/tecnote/c-shapes-proportionally-resize-with-window-in-sfml-2-x/

int main(int argc, char* argv[]) {
    Core core;
    
    sf::Texture playerSheet;
    if (!playerSheet.loadFromFile("resources/drawable/playerSheet.png"))
        throw std::runtime_error("Can't load player sprites");
    Player* playerIdle1 = new Player(playerSheet, sf::IntRect(13, 6, 21, 32), core.getMap(), core.getTiles());
    core.registerActor(playerIdle1);
    playerIdle1->setCoreInstance(&core);


    sf::Font defaultFont;
    if (!defaultFont.loadFromFile("resources/fonts/default.ttf"))
        throw std::runtime_error("Can't load default font");

    core.process();
    delete playerIdle1;
}
