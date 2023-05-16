#pragma once

#include <string>
#include <map>
#include <SFML/System/Clock.hpp>
#include "object.h"
#include "objects/fpsCounter.h"  // + sf::String, sf::Text, sf::Color


class Text;

class DebugInformer : public Object {
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

 private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void updateDebugString();

    sf::Font* m_font;
    sf::Clock m_updateTimer;
    FpsCounter* m_fpsCounter;
    float m_fps;

    Text* m_debugTextBlock;
    sf::String m_debugTextString;
    std::map<std::string, sf::String> m_additionInfo;

    const unsigned int m_textSize = 15;
    const sf::Vector2f m_anchorPos {5, 0};
};
