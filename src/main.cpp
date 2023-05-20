#include <cstdint>
#include <iostream>
#include "SFML/System/Vector2.hpp"
#include "core.h"
#include "objects/player.h"
#include "text.h"
#include "actor.h"
#include "objects/fpsCounter.h"
#include "objects/pauseScreen.h"

// marks:
// https://itecnote.com/tecnote/c-shapes-proportionally-resize-with-window-in-sfml-2-x/

int main(int argc, char* argv[]) {
    Core core;
    
    sf::Texture playerSheet;
    if (!playerSheet.loadFromFile("resources/drawable/playerSheet.png"))
        throw std::runtime_error("Can't load player sprites");
    Player* playerIdle1 = new Player(playerSheet,
        sf::IntRect(0, 0, 50, 40    ), sf::Vector2f(22, 32));

    sf::Vector2f playerStartPos(100, 100);
    playerIdle1->setStartPoint(playerStartPos);
    playerIdle1->setPosition(playerStartPos);
    core.registerObject(playerIdle1);
    playerIdle1->setCoreInstance(&core);

    core.process();
    delete playerIdle1;
}
