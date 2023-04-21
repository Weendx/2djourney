#pragma once

#include <string>
#include "../text.h"

class FpsCounter : public Text {
 public:
    explicit FpsCounter(const sf::Font& font);
    virtual ~FpsCounter() {}
    void onUpdate(const sf::Time &deltaTime) override;
    void handleEvent(const sf::Event& event) override {}

    void setFps(const std::string &fps);
    void setFps(const int &fps);
    void setFps(const float &fps);
 private:
    std::string m_fps;
};
