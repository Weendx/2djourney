#pragma once

#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "actor.h"

class Player: public Actor {
 public:
    Player(const sf::Texture& texture, const sf::IntRect& rectangle);
    void move();
    
    void onUpdate(const sf::Time &deltaTime) override;
    void handleEvent(sf::Event &event) override;
 private:
    sf::Vector2f m_deltaPos;
    float m_speed;
};
