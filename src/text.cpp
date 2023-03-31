#include "text.h"

Text::Text(const sf::Font &font) {
    m_drawable = new sf::Text("", font);
}

Text::Text(const sf::String &text, const sf::Font &font) {
    m_drawable = new sf::Text(text, font);
}

Text::Text(const Text &right) {
    setString(right.m_drawable->getString());
    setFillColor(right.m_drawable->getFillColor());
}

void Text::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(*m_drawable, states);
}

Text::operator std::string() const {
    return "Text [" + m_drawable->getString() + "]";
}
