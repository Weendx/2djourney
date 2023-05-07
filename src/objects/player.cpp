#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "core.h"
#include "objects/player.h"
#include "objects/tile.h"
#include <iostream>
#include <string>
#include "box2d/b2_body.h"
#include "utils.h"

Player::Player(const sf::Texture& texture, const sf::IntRect& rectangle,
                                                const sf::Vector2f& hitbox)
        : Actor(texture, rectangle, hitbox), m_velocity(0.35, 0.35) { 
    setName("Player"); 
    adjustScale({2.2, 2.2});

    sf::Vector2f playerSize = this->getGlobalBounds().getSize();
    // this->setOrigin(playerSize.x / 2, playerSize.y / 2);

    debugRect = new sf::RectangleShape;
    debugRect->setFillColor(sf::Color::Transparent);
    debugRect->setOutlineColor(sf::Color::White);
    debugRect->setOutlineThickness(1);
    debugRect->setSize({5, 5});
    debugRect->setOrigin({2.5, 2.5});

    debugRect2 = new sf::RectangleShape;
    debugRect2->setFillColor(sf::Color::Transparent);
    debugRect2->setOutlineColor(sf::Color::Red);
    debugRect2->setOutlineThickness(-1);
    debugRect2->setSize(playerSize);
    debugRect2->setOrigin(playerSize.x / 2, playerSize.y / 2);
    m_isDynamic = true;
}

Player::~Player() {
    delete debugRect;
}

void Player::movement(const float& milliseconds) {
    sf::Vector2f newPos = getPosition();
    m_velocity.x = m_velocity.y = 0.0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        m_velocity.x += -m_moveSpeed * milliseconds;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        m_velocity.x += m_moveSpeed * milliseconds;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        m_velocity.y += -m_moveSpeed * milliseconds;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        m_velocity.y += m_moveSpeed * milliseconds;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) /* && onGround(720) */ && !m_isJumping) {
        m_isJumping = true;
        m_velocity.y = -1*(m_velocity.y + 0.1 * milliseconds);
    }
    if (sf::Event::KeyReleased) {
        m_isJumping = false;
    }
    if (m_coreInstance) {
        sf::Vector2f hitbox = getHitBoxSize();
        sf::Vector2f searchPos = newPos;
        searchPos.y += hitbox.y / 2 + 5;
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


void Player::onUpdate(const sf::Time &deltaTime) {
    movement(deltaTime.asMilliseconds());
    // this->move(m_velocity);
    b2Vec2 vel;
    vel.Set(m_velocity.x * 25, m_velocity.y * 25);
    m_body->ApplyForceToCenter(vel, true);

    auto newWorldPos = m_body->GetPosition();
    this->setPosition(coordWorldToPixels(newWorldPos));
    this->setRotation(radToDeg(m_body->GetAngle()));
    
    if (m_coreInstance) {
        m_coreInstance->debug()->updateDebugString("Player World Pos",
            "("+ to_string_with_precision(newWorldPos.x, 2)
            + ", "+ to_string_with_precision(newWorldPos.y, 2) +")");
    }
}

void Player::handleEvent(const sf::Event &event) {}

bool Player::onGround(const unsigned int screenHeight) const {
    sf::Vector2f bottomCoords;
    bottomCoords.x = this->getGlobalBounds().left;
    bottomCoords.y = this->getGlobalBounds().top + this->getGlobalBounds().height + 10.0;
    if ((m_gameMap->getTileTypeAt(bottomCoords) != TileType::Empty) || (this->getPosition().y + this->getGlobalBounds().height + 1 > screenHeight)) {
        std::cout << "on ground" << std::endl;
        return true;
    } else {
        std::cout << "not on ground" << std::endl;
        return false;
    }
}
