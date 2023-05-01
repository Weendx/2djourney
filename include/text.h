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
    explicit Text(const sf::Font &font);
    Text(const sf::String &text, const sf::Font &font);
    Text(const sf::String &text, const sf::Font &font, 
                        unsigned const int &characterSize);
    virtual ~Text() {}

    void adjustScale(const sf::Vector2f &factors) override;
    operator std::string() const override; 
    void onUpdate(const sf::Time &deltaTime) override;
    void handleEvent(const sf::Event &event) override;

 private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif  // TEXT_H
