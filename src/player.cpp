#include "player.h"
#include "SFML/System/Vector2.hpp"

Player::Player(const sf::Texture& texture, const sf::IntRect& rectangle)
    : Actor(texture, rectangle), m_speed(0.4), m_deltaPos(0, 0) {}

void Player::onUpdate(const sf::Time &deltaTime) {
    if (m_deltaPos.x != 0) {
        sf::Vector2f newPosition(this->getPosition());
        newPosition.x += m_deltaPos.x * m_speed * deltaTime.asMilliseconds();
        this->setPosition(newPosition);
        m_deltaPos.x = 0;
    }
}

void Player::handleEvent(sf::Event &event) {
    if (event.type == sf::Event::KeyPressed) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            m_deltaPos.x = m_speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            m_deltaPos.x = -m_speed;
    }
}
