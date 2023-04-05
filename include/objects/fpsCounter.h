#pragma once

#include "../text.h"
#include <string>

class FpsCounter : public Text {
 public:
    FpsCounter(const sf::Font& font);
    virtual ~FpsCounter() {}
    void onUpdate(const sf::Time &deltaTime) override;
    void handleEvent(sf::Event&) override {}

    void setFps(const std::string &fps);
    void setFps(const int &fps);
    void setFps(const float &fps);
 private:
    std::string m_fps;
};
