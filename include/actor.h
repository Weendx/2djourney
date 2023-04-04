#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "object.h"

class Actor : virtual public Object<sf::Sprite> {
 public:
    Actor(const sf::Texture &texture, const sf::IntRect &rectangle);
    Actor(const Actor& right);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    operator std::string() const override;
};

#endif  // ACTOR_H
