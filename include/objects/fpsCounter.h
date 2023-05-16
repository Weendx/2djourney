#pragma once

#include <string>
#include "../text.h"

class FpsCounter : public Text {
 public:
    explicit FpsCounter(const sf::Font& font);
    FpsCounter(const sf::Font& font, unsigned const int& textSize);
    virtual ~FpsCounter() {}
    void onUpdate(const sf::Time &deltaTime) override;
    void handleEvent(const sf::Event& event) override {}

    void setFps(const std::string &fps);
    void setFps(const int &fps);
    void setFps(const float &fps);
 private:
    std::string m_fps;
};
