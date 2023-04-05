#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "object.h"

class Actor 
   : virtual public Object, public sf::Sprite {
 public:
    Actor(const sf::Texture& texture, const sf::IntRect& rectangle);
    virtual ~Actor() {} 
    operator std::string() const override;
    void adjustScale(const sf::Vector2f &factors) override;
 private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif  // ACTOR_H
