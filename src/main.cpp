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
    Player* playerIdle1 = new Player(playerSheet, 
                        sf::IntRect(0, 0, 50, 37), sf::Vector2f(22, 32));
    playerIdle1->setPosition(100, 100);
    core.registerActor(playerIdle1);
    playerIdle1->setCoreInstance(&core);

    sf::Font defaultFont;
    if (!defaultFont.loadFromFile("resources/fonts/default.ttf"))
        throw std::runtime_error("Can't load default font");

    core.process();
    delete playerIdle1;
}
