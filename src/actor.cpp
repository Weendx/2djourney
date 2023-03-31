
#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "actor.h"

Actor::Actor(const Actor& right) {
    delete m_drawable;
    m_drawable = new sf::Sprite();
    setName("Copied");
}

Actor::Actor(const sf::Texture &texture, const sf::IntRect &rectangle) {
    m_drawable = new sf::Sprite (texture, rectangle);
    m_drawable->setPosition(50, 50);
    setName("Actor");
}

void Actor::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(*m_drawable, states);
}

Actor::operator std::string() const {
    return "Actor [" + m_name + "]";
}
