#include "text.h"

Text::Text(const sf::Font &font): sf::Text("", font) {}

Text::Text(const sf::String &text, const sf::Font &font)
    : sf::Text(text, font) {}
//
// Text::Text(const Text &right) {
//     setString(right.m_drawable->getString());
//     setFillColor(right.m_drawable->getFillColor());
// }
//
// void Text::draw(sf::RenderTarget &target, sf::RenderStates states) const {
//     target.draw(*m_drawable, states);
// }
void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw((sf::Text)*this, states);
}


void Text::adjustScale(const sf::Vector2f &factors) {
    sf::Vector2f tmpScale(1.0, 1.0);
    tmpScale = this->getScale();
    tmpScale.x *= factors.x;
    tmpScale.y *= factors.y;
    this->setScale(tmpScale);
}

Text::operator std::string() const {
    return "Text []";
}
