#pragma once

#include <string>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "object.h"

class Actor 
    : virtual public Object, public sf::Sprite {
 public:
    Actor(const sf::Texture& texture, const sf::IntRect& rectangle, 
                                        const bool &hasPhysics = true);
    Actor(const sf::Texture& texture, const sf::IntRect& rectangle, 
            const sf::Vector2f& hitboxSizes, const bool &hasPhysics = true);
    virtual ~Actor() {} 
    operator std::string() const override;
    void adjustScale(const sf::Vector2f &factors) override;
    sf::Vector2f getHitBoxSize() override;
    void addPhysics(b2World* world) override;
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void init();
    sf::Vector2f m_hitboxSizes;
};

