#pragma once

#include <string>
#include <map>
#include <SFML/System/Clock.hpp>
#include "object.h"
#include "objects/fpsCounter.h"  // + sf::String, sf::Text, sf::Color


class Text;

class DebugInformer : virtual public Object {
 public:
    DebugInformer();
    ~DebugInformer();
    void onUpdate(const sf::Time& deltaTime) override;
    void handleEvent(const sf::Event& event) override;
    void adjustScale(const sf::Vector2f &factors) override;
    operator std::string() const override { return "DebugInformer"; }
    
    void setFPS(const float& fps);
    void addDebugString(const std::string& key, const sf::String& text);
    void updateDebugString(const std::string& key, const sf::String& text);
    void updateString(const sf::String& key, const sf::String& text);

 private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void updateTextStrings();
    void adjustDebugBlock();

    sf::Font* m_font;
    sf::Clock m_updateTimer;
    FpsCounter* m_fpsCounter;
    float m_fps;

    Text* m_debugTextBlock;
    Text* m_userTextBlock;
    sf::String m_debugTextString;
    sf::String m_userTextString;
    std::map<std::string, sf::String> m_additionInfo;
    std::map<sf::String, sf::String> m_userInfo;

    const unsigned int m_textSize = 15;
    const sf::Vector2f m_anchorPos {5, 0};  // global
    sf::Vector2f m_textAnchor;  // for debug & user Text*
};
