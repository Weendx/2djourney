#pragma once

#include <string>
#include "object.h"

namespace sf {
class RectangleShape;
class Font;
class String;
}
class Text;

class PauseScreen : virtual public Object {
 public:
    PauseScreen(Core* coreInstance, const sf::Font& font);
    virtual ~PauseScreen();
    void adjustScale(const sf::Vector2f &factors) override;
    operator std::string() const override;
    void onUpdate(const sf::Time &deltaTime) override;
    void onRestart() override;
    void handleEvent(const sf::Event &event) override;
    void setActive();
    void setMainText(const sf::String& text);

 private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    sf::RectangleShape* m_bg;
    Text* m_descText;
    Text* m_mainText = nullptr;
    sf::Vector2f m_scale;

    bool m_isActive;
};
