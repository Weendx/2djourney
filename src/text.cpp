#include "text.h"

Text::Text(const sf::Font &font): sf::Text("", font) {}

Text::Text(const sf::String &text, const sf::Font &font)
    : sf::Text(text, font) {}

Text::Text(const sf::String &text, const sf::Font &font, 
                            const unsigned int &characterSize)
    : sf::Text(text, font, characterSize) {}

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

void Text::onUpdate(const sf::Time& deltaTime) {}
void Text::handleEvent(const sf::Event &event) {}
