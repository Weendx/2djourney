#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "core.h"
#include "objects/player.h"
#include "objects/tile.h"

Player::Player(const sf::Texture& texture, const sf::IntRect& rectangle)
                        : Actor(texture, rectangle), m_velocity(0.35, 0.35) { 
    setName("Player"); 
    setScale(2.2, 2.2);

    sf::Vector2f playerSize = this->getGlobalBounds().getSize();
    // this->setOrigin(playerSize.x / 2, playerSize.y / 2);

    debugRect = new sf::RectangleShape;
    debugRect->setFillColor(sf::Color::Transparent);
    debugRect->setOutlineColor(sf::Color::White);
    debugRect->setOutlineThickness(1);
    debugRect->setSize({5, 5});

    debugRect2 = new sf::RectangleShape;
    debugRect2->setFillColor(sf::Color::Transparent);
    debugRect2->setOutlineColor(sf::Color::Red);
    debugRect2->setOutlineThickness(1);
    debugRect2->setSize(playerSize);
    // debugRect2->setOrigin(playerSize.x / 2, playerSize.y / 2);
}

Player::~Player() {
    delete debugRect;
}

void Player::movement(const float& milliseconds) {
    sf::Vector2f newPos = getPosition();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        newPos.x += -m_velocity.x * milliseconds;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        newPos.x += m_velocity.x * milliseconds;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        newPos.y += -m_velocity.y * milliseconds;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        newPos.y += m_velocity.y * milliseconds;
    }
    if (m_coreInstance) {
        sf::Vector2f playerSize = getGlobalBounds().getSize();
        sf::Vector2f searchPos = newPos + playerSize;
        searchPos.x -= playerSize.x / 2;
        debugRect->setPosition(searchPos);
        auto ttype = tileTypeToString(
                    m_coreInstance->getMap()->getTileTypeAt(searchPos));
        m_coreInstance->debug()->updateDebugString("Ground Type", ttype);
    }
    setPosition(newPos);
    debugRect2->setPosition(newPos);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw((sf::Sprite) *this, states);
    target.draw(*debugRect, states);
    target.draw(*debugRect2, states);
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
