#include <cstdint>
#include <iostream>
#include "core.h"
#include "actor.h"
#include "text.h"

// marks:
// https://itecnote.com/tecnote/c-shapes-proportionally-resize-with-window-in-sfml-2-x/

int main(int argc, char* argv[]) {
    Core core;
    
    sf::Texture playerSheet;
    if (!playerSheet.loadFromFile("resources/drawable/playerSheet.png"))
        throw std::runtime_error("Can't load player sprites");
    Actor* playerIdle1 = new Actor(playerSheet, sf::IntRect(0, 0, 50, 37));
    core.registerObject(playerIdle1);

    core.setScale(2.0, 2.0);

    core.process();
    delete playerIdle1;
}
