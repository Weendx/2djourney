#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include "object.h"

class Text: public Object<sf::Text> {
 public:
    Text() = delete;
    Text(const sf::Font &font);
    Text(const sf::String &text, const sf::Font &font);
    Text(const Text& right);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setString(const sf::String &text) { m_drawable->setString(text); }
    
    void setFillColor(const sf::Color &color) { 
        m_drawable->setFillColor(color); 
    }
    
    operator std::string() const override;
};

#endif  // TEXT_H
