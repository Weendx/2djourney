#pragma once

#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/View.hpp"
#include "SFML/System/Clock.hpp"
#include "box2d/b2_world_callbacks.h"
#include "box2d/b2_contact.h"
#include "actor.h"
#include "objects/map.h"

namespace sf {
class RectangleShape;
}

enum PlayerStates {IDLE = 0, MOVING_LEFT, MOVING_RIGHT, JUMPING_LEFT,JUMPING_RIGHT, FALLING_LEFT, FALLING_RIGHT};
enum LastDirection {LEFT = 0, RIGHT};

class Player : public Actor {
 public:
    Player(const sf::Texture& texture, const sf::IntRect& rectangle,
                            const sf::Vector2f& hitboxSizes = sf::Vector2f());
    ~Player();
    void updateMovement(const float& milliseconds);
    void updateAnimations();
    void onUpdate(const sf::Time& deltaTime) override;
    void onRestart() override;
    void handleEvent(const sf::Event& event) override;
    void addPhysics(b2World* world) override;
    bool isOnGround();
    void setStartPoint(const sf::Vector2f& pixelCoords);
    void setStartPoint(const b2Vec2& worldCoords);
    void setViewPosition() {}
    void setPlayerSheet();
 private:
    sf::Vector2f m_startPoint;

    sf::Event m_event;

    //Animation
    sf::Texture m_playerSheet;
    sf::Clock m_animationTimer;
    short m_state;
    short m_lastDirection;
    sf::IntRect m_currentFrame;
    bool animationSwitch;

    // Physics
    const float m_moveSpeed = 0.01;
    sf::Vector2f m_velocity;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::RectangleShape* searchRect;
    sf::RectangleShape* spriteRect;

    b2ContactListener* m_contactListener = nullptr;
};

class PlayerContactListener : public b2ContactListener {
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
};
