#pragma once

#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "actor.h"

class Player: public Actor {
 public:
    Player(const sf::Texture& texture, const sf::IntRect& rectangle);
    void movement(const float& milliseconds);
    // void physicsParameters();
    // void physics(const float& milliseconds);
    void onUpdate(const sf::Time &deltaTime) override;
    void handleEvent(const sf::Event &event) override;
    void screenCollision(const unsigned int screenWidth, 
                            const unsigned int screenHeight);
    // sf::FloatRect getBounds();
    // sf::FloatRect getNextPosition();
 private:
    sf::Vector2f velocity;
    // float velocityMax;
    // float acceleration;
    // float gravity;
};
