#pragma once

#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/View.hpp"
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
    bool onGround(const unsigned int screenHeight)const;
    void setViewPosition() {

    }
private:
    const Map* m_gameMap;
    // const std::vector<Tile*> m_tiles;

    //Physics
    const float m_moveSpeed = 0.3;
    sf::Vector2f m_velocity;
    const float m_gravitySpeed = 10.0;
    const float m_accelerationY = 1.0;
    bool m_isJumping = false;
    sf::FloatRect m_nextPos;
    //sf::RectangleShape m_hitbox;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::RectangleShape* debugRect;
    sf::RectangleShape* debugRect2;

};
