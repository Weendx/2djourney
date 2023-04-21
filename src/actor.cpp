
#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "actor.h"


Actor::Actor(const sf::Texture& texture, const sf::IntRect& rectangle)
    : sf::Sprite(texture, rectangle) {}

void Actor::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw((sf::Sprite)*this, states);
}

void Actor::adjustScale(const sf::Vector2f &factors) {
    sf::Vector2f tmpScale(1.0, 1.0);
    tmpScale = this->getScale();
    tmpScale.x *= factors.x;
    tmpScale.y *= factors.y;
    this->setScale(tmpScale);
}

Actor::operator std::string() const {
    return "Actor [" + m_name + "]";
}
