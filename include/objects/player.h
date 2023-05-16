#pragma once

#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/View.hpp"
#include "box2d/b2_world_callbacks.h"
#include "box2d/b2_contact.h"
#include "actor.h"
#include "objects/map.h"

namespace sf {
class RectangleShape;
}

class Player : public Actor {
public:
    Player(const sf::Texture& texture, const sf::IntRect& rectangle,
                            const sf::Vector2f& hitboxSizes = sf::Vector2f());
    ~Player();
    void movement(const float& milliseconds);
    void onUpdate(const sf::Time& deltaTime) override;
    void handleEvent(const sf::Event& event) override;
    void addPhysics(b2World* world) override;
    bool isOnGround();
    void setViewPosition() {

    }
private:
    const Map* m_gameMap;
    // const std::vector<Tile*> m_tiles;

    //Physics
    const float m_moveSpeed = 1.0;
    sf::Vector2f m_velocity;
    const float m_gravitySpeed = 10.0;
    const float m_accelerationY = 1.0;
    bool m_isJumping = false;
    sf::FloatRect m_nextPos;
    //sf::RectangleShape m_hitbox;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::RectangleShape* searchRect;
    sf::RectangleShape* spriteRect;

    b2ContactListener* m_contactListener = nullptr;
    int m_contactsCount = 0;
};

class PlayerContactListener : public b2ContactListener {
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
};
