#pragma once

#include <string>
#include <SFML/System/Clock.hpp>
#include "object.h"
#include "objects/fpsCounter.h"

class DebugInformer : public Object {
 public:
    DebugInformer();
    ~DebugInformer();
    void onUpdate(const sf::Time& deltaTime) override;
    void handleEvent(const sf::Event& event) override;
    void adjustScale(const sf::Vector2f &factors) override {}
    operator std::string() const override { return "DebugInformer"; }
    void setFPS(const float& fps);
 private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::Font* m_font;
    sf::Clock m_updateTimer;
    FpsCounter* m_fpsCounter;
    float m_fps;
};
