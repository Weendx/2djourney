#include "objects/player.h"
#include "SFML/System/Vector2.hpp"

Player::Player(const sf::Texture& texture, const sf::IntRect& rectangle)
                        : Actor(texture, rectangle), m_velocity(0.35, 0.35) { 
    setName("Player"); 
    setScale(2.2, 2.2);
}

void Player::movement(const float& milliseconds) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        this->move(-m_velocity.x * milliseconds, 0.0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        this->move(m_velocity.x * milliseconds, 0.0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        this->move(0.0, -m_velocity.y * milliseconds);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        this->move(0.0, m_velocity.y * milliseconds);
    }
}

// void Player::physicsParameters() {
//    this->velocityMax = 10.0;
//    
//    this->gravity = 2.0;
// }
//
// void Player::physics(const float& milliseconds) {
//    //gravity
//    this->velocity.y += 1.0 * this->gravity;
// }

void Player::onUpdate(const sf::Time &deltaTime) {
    screenCollision(1280, 720);
    movement(deltaTime.asMilliseconds());
    /*physics(deltaTime.asMilliseconds());*/
}

void Player::handleEvent(const sf::Event &event) {}

void Player::screenCollision(const unsigned int screenWidth, 
        const unsigned int screenHeight) {
    // left
    if (this->getPosition().x < 0.0)
        this->setPosition(0.0, this->getPosition().y);

    // right
    if (this->getPosition().x + this->getGlobalBounds().width > screenWidth)
        this->setPosition(screenWidth - this->getGlobalBounds().width, 
                                                    this->getPosition().y);

    // top
    if (this->getPosition().y < 0.0)
        this->setPosition(this->getPosition().x, 0.0);

    // bottom
    if (this->getPosition().y + this->getGlobalBounds().height > screenHeight)
        this->setPosition(this->getPosition().x, 
                            screenHeight - this->getGlobalBounds().height);
}

// sf::FloatRect Player::getBounds() {
//    return this->getGlobalBounds();
// }
//
// sf::FloatRect Player::getNextPosition() {
//    return this->getBounds().left + velocity;
// }
