#include "box2d/b2_math.h"
#include "box2d/b2_polygon_shape.h"
#include "utils.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "core.h"
#include "objects/player.h"
#include "objects/tile.h"
#include <cstdint>
#include <iostream>
#include <string>
#include "box2d/b2_body.h"
#include <box2d/b2_world_callbacks.h>
#include <box2d/b2_contact.h>

namespace {
int contactsCount = 0;
}

Player::Player(const sf::Texture& texture, const sf::IntRect& rectangle,
                                                const sf::Vector2f& hitbox)
        : Actor(texture, rectangle, hitbox), m_velocity(0.0, 0.0) { 
    setName("Player"); 
    adjustScale({2.2, 2.2});

    sf::Vector2f playerSize = this->getGlobalBounds().getSize();
    // this->setOrigin(playerSize.x / 2, playerSize.y / 2);

    searchRect = new sf::RectangleShape;
    searchRect->setFillColor(sf::Color::Transparent);
    searchRect->setOutlineColor(sf::Color::White);
    searchRect->setOutlineThickness(1);
    searchRect->setSize({5, 5});
    searchRect->setOrigin({2.5, 2.5});

    spriteRect = new sf::RectangleShape;
    spriteRect->setFillColor(sf::Color::Transparent);
    spriteRect->setOutlineColor(sf::Color::Red);
    spriteRect->setOutlineThickness(-1);
    spriteRect->setSize(playerSize);
    spriteRect->setOrigin(playerSize.x / 2, playerSize.y / 2);
    m_isDynamic = true;
}

Player::~Player() {
    delete searchRect;
    if (m_contactListener)
        delete m_contactListener;
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
    /*if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        m_velocity.y += -m_moveSpeed * milliseconds;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        m_velocity.y += m_moveSpeed * milliseconds;
    }*/
    if (!m_isJumping && sf::Keyboard::isKeyPressed(sf::Keyboard::Space) /* && onGround(720) */) {
        m_isJumping = true;
        b2Vec2 vel(0, -0.25f);
        m_body->ApplyLinearImpulseToCenter(vel, true);
        // m_velocity.y = -1*(m_velocity.y + 0.1 * milliseconds);
    }
    if (sf::Event::KeyReleased) {
        m_isJumping = false;
    }
    if (m_coreInstance) {
        sf::Vector2f hitbox = getHitBoxSize();
        sf::Vector2f searchPos = newPos;
        searchPos.y += hitbox.y / 2 + 15;
        searchRect->setPosition(searchPos);
        auto ttype = tileTypeToString(
                    m_coreInstance->getMap()->getTileTypeAt(searchPos));
        m_coreInstance->debug()->updateDebugString("Ground Type", ttype);
        m_coreInstance->debug()->updateDebugString("IsPlayerOnGround", 
            isOnGround() ? "Yes" : "No");
    }
    setPosition(newPos);
    spriteRect->setPosition(newPos);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw((sf::Sprite) *this, states);
    if (showDebug) {
        target.draw(*searchRect, states);
        target.draw(*spriteRect, states);
    }
}


void Player::onUpdate(const sf::Time &deltaTime) {
    movement(deltaTime.asMilliseconds());
    this->move(m_velocity);
    b2Vec2 vel;
    int scale = 1;
    vel.Set(m_velocity.x * scale, m_velocity.y * scale);
    // Добавить всем этим штукам (см 6.f и в прыжке и m_velocity) 
    // связь с coordPixelsToWorld
    if (std::abs(m_body->GetLinearVelocity().x) < 6.f) {
        m_body->ApplyForceToCenter(vel, true);
    }

    auto newWorldPos = m_body->GetPosition();
    this->setPosition(coordWorldToPixels(newWorldPos));
    this->setRotation(radToDeg(m_body->GetAngle()));
    
   if (m_coreInstance) {
        m_coreInstance->debug()->updateDebugString("Player World Pos",
            "("+ to_string_with_precision(newWorldPos.x, 2)
            + ", "+ to_string_with_precision(newWorldPos.y, 2) +")");
    }
    if (m_coreInstance) {
        m_coreInstance->setPlayerCoords(getPosition());
    }
}

void Player::handleEvent(const sf::Event &event) {}

bool Player::isOnGround() {
    if (!m_contactListener) {
        m_contactListener = new PlayerContactListener();
    }
    m_coreInstance->getWorld()->SetContactListener(m_contactListener);
    return contactsCount > 0;
}

void Player::addPhysics(b2World* world) {
    b2BodyDef bdef;
    if (this->isItDynamic()) {
        bdef.type = b2_dynamicBody;
        bdef.fixedRotation = true;
    }

    bdef.position = coordPixelsToWorld(this->getPosition());
    b2Body* body = world->CreateBody(&bdef);

    b2PolygonShape ps;
    b2Vec2 hitboxsizes = coordPixelsToWorld(this->getHitBoxSize());
    ps.SetAsBox(hitboxsizes.x / 2.0f, hitboxsizes.y / 2.0f);


    b2FixtureDef fd;
    fd.shape = &ps;

    fd.density = 1;
    fd.friction = 50;
    fd.restitution = 0;

    body->CreateFixture(&fd);
    
    b2Vec2 sensorSize = coordPixelsToWorld(10, 10);
    ps.SetAsBox(sensorSize.x, sensorSize.y, b2Vec2(0, hitboxsizes.y / 2.0f), 0);
    fd.isSensor = true;
    b2FixtureUserData udata;
    udata.pointer = (uintptr_t) 10;
    fd.userData = udata;
    body->CreateFixture(&fd);

    this->setBody(body);
}

void PlayerContactListener::BeginContact(b2Contact* contact) {
    b2FixtureUserData& fixtureData1 = contact->GetFixtureA()->GetUserData();
    b2FixtureUserData& fixtureData2 = contact->GetFixtureB()->GetUserData();
    int fixtureId1 = static_cast<int>(fixtureData1.pointer);
    int fixtureId2 = static_cast<int>(fixtureData2.pointer);
    if (!fixtureId1 || !fixtureId2)
        return;
    if (fixtureId1 == 99 && fixtureId2 == 10)
        ++contactsCount;
}
void PlayerContactListener::EndContact(b2Contact* contact) {
    b2FixtureUserData& fixtureData1 = contact->GetFixtureA()->GetUserData();
    b2FixtureUserData& fixtureData2 = contact->GetFixtureB()->GetUserData();
    int fixtureId1 = static_cast<int>(fixtureData1.pointer);
    int fixtureId2 = static_cast<int>(fixtureData2.pointer);
    if (!fixtureId1 || !fixtureId2)
        return;
    if (fixtureId1 == 99 && fixtureId2 == 10)
        --contactsCount;
}
