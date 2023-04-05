#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include "object.h"

class Text: virtual public Object, public sf::Text {
 public:
    Text(const sf::Font &font);
    Text(const sf::String &text, const sf::Font &font);
    Text(const sf::String &text, const sf::Font &font, const unsigned int &characterSize);
    virtual ~Text() {}

    void adjustScale(const sf::Vector2f &factors) override;
    operator std::string() const override; 
 private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif  // TEXT_H
