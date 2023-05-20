#include "objects/pauseScreen.h"
#include <cmath>
#include <stdexcept>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/String.hpp>

#include "text.h"
#include "core.h"

PauseScreen::PauseScreen(Core* coreInstance, const sf::Font& font) 
        : m_bg(new sf::RectangleShape()), m_isActive(true), m_scale(1, 1) {
    setCoreInstance(coreInstance);
    std::wstring sometext = L"\u0414\u043b\u044f\u0020\u043f\u0440\u043e\u0434\u043e\u043b\u0436\u0435\u043d\u0438\u044f\u0020\u043d\u0430\u0436\u043c\u0438\u0442\u0435\u0020\u043a\u043b\u0430\u0432\u0438\u0448\u0443\u002e\u002e\u002e      ";
    sf::String desctext(sometext);
    m_descText = new Text(desctext, font, 30);

    sf::Vector2f windowSize = m_coreInstance->getWindowSize();
    m_bg->setSize(windowSize);
    m_bg->setFillColor(sf::Color(0, 0, 0, 150));
    
    sf::Vector2f textSize = m_descText->getGlobalBounds().getSize();
    m_descText->setOrigin(textSize / 2.f);
    m_descText->setPosition(windowSize.x / 2.f, windowSize.y * 0.75f);
}

PauseScreen::~PauseScreen() {
    delete m_bg;
    if (m_descText)
        delete m_descText;
}

namespace {
float textscale = 1.0;
bool isRising = true;
}

void PauseScreen::onUpdate(const sf::Time &deltaTime) {
    if (!m_isActive)
        return;
    float step = 0.5 * deltaTime.asMilliseconds() * 0.0001;
    textscale += isRising ? step : -step;

    if (textscale > 1.05)
        isRising = false;
    else if (textscale < 0.9)
        isRising = true;

    m_descText->setScale(m_scale * textscale);
}

void PauseScreen::onRestart() {
    setActive();
}

void PauseScreen::handleEvent(const sf::Event &event) {
    if (event.type == sf::Event::EventType::KeyPressed) {
        if (event.key.code == sf::Keyboard::R)
            return;
        if (m_isActive) {
            m_coreInstance->unsetPause();
            m_isActive = false;
        } else if (event.key.code == sf::Keyboard::P) {
            m_isActive = true;
            m_coreInstance->setPause();
        }
    }
}

void PauseScreen::setActive() {
    m_isActive = true;
    m_coreInstance->setPause();
}

void PauseScreen::draw(sf::RenderTarget &target, 
                        sf::RenderStates states) const {
    if (!m_isActive)
        return;
    target.draw(*m_bg);
    target.draw((sf::Text) *m_descText);
}

void PauseScreen::adjustScale(const sf::Vector2f &factors) {
    m_scale.x *= factors.x;
    m_scale.y *= factors.y;
}

PauseScreen::operator std::string() const {
    return "Object<PauseScreen>";
}
