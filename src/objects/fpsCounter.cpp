#include "objects/fpsCounter.h"

FpsCounter::FpsCounter(const sf::Font &font) 
    : Text("FPS: 0", font, 15), m_fps("0") {}

FpsCounter::FpsCounter(const sf::Font &font, const unsigned int& textSize) 
    : Text("FPS: 0", font, textSize), m_fps("0") {}

    void FpsCounter::onUpdate(const sf::Time &deltaTime) {
    this->setString("FPS: " + m_fps);
}

void FpsCounter::setFps(const std::string &fps) {
    m_fps = fps;
    this->setString("FPS: " + m_fps);
}
void FpsCounter::setFps(const int &fps) {
    m_fps = std::to_string(fps);
    this->setString("FPS: " + m_fps);
}
void FpsCounter::setFps(const float &fps) {
    m_fps = std::to_string((int8_t) fps);
    this->setString("FPS: " + m_fps);
}
