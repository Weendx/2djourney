#include "objects/player.h"
#include <box2d/b2_math.h>
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_contact.h>
#include <box2d/b2_world_callbacks.h>

#include <cstdint>
#include <iostream>
#include <string>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "core.h"
#include "objects/tile.h"
#include "utils.h"


namespace {
int contactsCount = 0;
bool isDied = false;
}

Player::Player(const sf::Texture& texture, const sf::IntRect& rectangle,
                                                const sf::Vector2f& hitbox)
        : Actor(texture, rectangle, hitbox),
          m_state(IDLE), m_currentFrame(sf::IntRect(0,0,50,35)) { 
    setName("Player"); 
    adjustScale({2.2, 2.2});
    m_animationTimer.restart();
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

void Player::updateMovement(const float& milliseconds) {
    sf::Vector2f newPos = getPosition();
    m_state = IDLE;
    
    b2Vec2 curVel;
    curVel.Set(m_body->GetLinearVelocity().x, m_body->GetLinearVelocity().y);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {;
        m_body->SetLinearVelocity(b2Vec2(-6.0, curVel.y));
        m_lastDirection = LEFT;
        m_state = MOVING_LEFT;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        m_body->SetLinearVelocity(b2Vec2(6.0, curVel.y));
        m_lastDirection = RIGHT;
        m_state = MOVING_RIGHT;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isOnGround()) {
        b2Vec2 vel(0, -2.4f);
        m_body->ApplyLinearImpulseToCenter(vel, true);
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

void Player::updateAnimations() {
    if (!(this->isOnGround())) {
        if (m_body->GetLinearVelocity().y < 0) {
            if (m_body->GetLinearVelocity().x < 0 || m_lastDirection == LEFT)
                m_state = JUMPING_LEFT;
            if (m_body->GetLinearVelocity().x > 0 || m_lastDirection == RIGHT)
                m_state = JUMPING_RIGHT;
        }
        if (m_body->GetLinearVelocity().y >= 0) {
            if (m_body->GetLinearVelocity().x < 0 || m_lastDirection == LEFT)
                m_state = FALLING_LEFT;
            if (m_body->GetLinearVelocity().x > 0 || m_lastDirection == RIGHT)
                m_state = FALLING_RIGHT;
        }
    }

    if (m_state == IDLE && this->isOnGround()) {
        if (m_animationTimer.getElapsedTime().asSeconds() >= 0.2f) {
            m_currentFrame.top = 0.f;
            m_currentFrame.left += 50.f;
            if (m_currentFrame.left > 150.f)
                this->m_currentFrame.left = 0;

            m_animationTimer.restart();
            this->setTextureRect(m_currentFrame);
        }
    }
    else if (m_state == MOVING_RIGHT && this->isOnGround()) {
        if (m_animationTimer.getElapsedTime().asSeconds() >= 0.1f) {
            m_currentFrame.top = 40.f;
            m_currentFrame.left += 50.f;
            if (m_currentFrame.left > 250.f)
                m_currentFrame.left = 0;

            m_animationTimer.restart();
            this->setTextureRect(m_currentFrame);
        }
        this->setScale(2.2f, 2.2f);
        this->setOrigin(this->getGlobalBounds().width / 4.f, this->getGlobalBounds().height / 4.f);
    }
    else if (m_state == MOVING_LEFT && this->isOnGround()) {
        if (m_animationTimer.getElapsedTime().asSeconds() >= 0.1f) {
            m_currentFrame.top = 40.f;
            m_currentFrame.left += 50.f;
            if (m_currentFrame.left > 250.f)
                m_currentFrame.left = 0;

            m_animationTimer.restart();
            this->setTextureRect(m_currentFrame);
        }
        this->setScale(-2.2f, 2.2f);
        this->setOrigin(this->getGlobalBounds().width / 4.f, this->getGlobalBounds().height / 4.f);
    }
    else if (m_state == JUMPING_RIGHT) {
        if (m_animationTimer.getElapsedTime().asSeconds() >= 0.08f) {
            m_currentFrame.top = 75.f;
            m_currentFrame.left += 50.f;
            if (m_currentFrame.left > 200.f)
                m_currentFrame.left = 0;

            m_animationTimer.restart();
            this->setTextureRect(m_currentFrame);
        }
        this->setScale(2.2f, 2.2f);
        this->setOrigin(this->getGlobalBounds().width / 4.f, this->getGlobalBounds().height / 4.f);
    }
    else if (m_state == JUMPING_LEFT) {
        if (m_animationTimer.getElapsedTime().asSeconds() >= 0.08f) {
            m_currentFrame.top = 75.f;
            m_currentFrame.left += 50.f;
            if (m_currentFrame.left > 200.f)
                m_currentFrame.left = 0;

            m_animationTimer.restart();
            this->setTextureRect(m_currentFrame);
        }
        this->setScale(-2.2f, 2.2f);
        this->setOrigin(this->getGlobalBounds().width / 4.f, this->getGlobalBounds().height / 4.f);
    }
    else if (m_state == FALLING_RIGHT) {
        if (m_animationTimer.getElapsedTime().asSeconds() >= 0.1f) {
            m_currentFrame.top = 75.f;
            m_currentFrame.left = 250.f;
            m_currentFrame.left += 50.f;
            if (m_currentFrame.left > 350.f)
                m_currentFrame.left = 250;

            m_animationTimer.restart();
            this->setTextureRect(m_currentFrame);
        }
        this->setScale(2.2f, 2.2f);
        this->setOrigin(this->getGlobalBounds().width / 4.f, this->getGlobalBounds().height / 4.f);
    }
    else if (m_state == FALLING_LEFT) {
        if (m_animationTimer.getElapsedTime().asSeconds() >= 0.1f) {
            m_currentFrame.top = 75.f;
            m_currentFrame.left = 250.f;
            m_currentFrame.left += 50.f;
            if (m_currentFrame.left > 350.f)
                m_currentFrame.left = 250;

            m_animationTimer.restart();
            this->setTextureRect(m_currentFrame);
        }
        this->setScale(-2.2f, 2.2f);
        this->setOrigin(this->getGlobalBounds().width / 4.f, this->getGlobalBounds().height / 4.f);
    }
    else {
        m_animationTimer.restart();
    }
}



void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw((sf::Sprite) *this, states);
    if (showDebug) {
        target.draw(*searchRect, states);
        target.draw(*spriteRect, states);
    }
}


void Player::onUpdate(const sf::Time &deltaTime) {
    if (m_coreInstance && isDied)
        m_coreInstance->restartGame();

    updateMovement(deltaTime.asMilliseconds());
    updateAnimations();
    handleEvent(m_event);

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

void Player::onRestart() {
    m_body->SetLinearVelocity(b2Vec2(0.0, 0.0));
    setPosition(m_startPoint);
    m_body->SetTransform(coordPixelsToWorld(m_startPoint), 0);
    m_body->SetLinearVelocity({0, 0});
    isDied = false;
}

void Player::handleEvent(const sf::Event& event) {
    b2Vec2 curVel;
    curVel.Set(m_body->GetLinearVelocity().x, m_body->GetLinearVelocity().y);
    if (event.type == sf::Event::EventType::KeyReleased) {
        if (event.key.code == sf::Keyboard::A) {
            curVel.x = 0.f;
            m_body->SetLinearVelocity(curVel);
        }
        if (event.key.code == sf::Keyboard::D) {
            curVel.x = 0.f;
            m_body->SetLinearVelocity(curVel);
        }
    }
}

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
    fd.friction = 1;
    fd.restitution = 0;

    body->CreateFixture(&fd);
    
    b2Vec2 collisionSensorSize = coordPixelsToWorld(22, 10);
    ps.SetAsBox(collisionSensorSize.x, collisionSensorSize.y, b2Vec2(0, hitboxsizes.y / 2.0f), 0);
    fd.isSensor = true;
    b2FixtureUserData udata1;
    udata1.pointer = (uintptr_t) 10;
    fd.userData = udata1;
    body->CreateFixture(&fd);


    b2Vec2 spikeSensorSize = coordPixelsToWorld(20, 5);
    ps.SetAsBox(spikeSensorSize.x, spikeSensorSize.y, b2Vec2(0, hitboxsizes.y / 2.0f), 0);
    fd.isSensor = true;
    b2FixtureUserData udata2;
    udata2.pointer = (uintptr_t) 20;
    fd.userData = udata2;
    
    body->CreateFixture(&fd);

    this->setBody(body);
}

void Player::setStartPoint(const sf::Vector2f& coords) {
    m_startPoint = coords;
}
void Player::setStartPoint(const b2Vec2& coords) {
    m_startPoint = coordWorldToPixels(coords);
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
    if (fixtureId1 == 12 && fixtureId2 == 20)
        isDied = true;        
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
