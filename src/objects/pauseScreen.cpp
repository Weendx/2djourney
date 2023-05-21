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

#include "SFML/System/Clock.hpp"
#include "text.h"
#include "core.h"

PauseScreen::PauseScreen(Core* coreInstance, const sf::Font& font) 
        : m_bg(new sf::RectangleShape()), m_isActive(true), m_scale(1, 1) {
    setCoreInstance(coreInstance);
    std::wstring sometext = L"\u0414\u043b\u044f\u0020\u043f\u0440\u043e"
        L"\u0434\u043e\u043b\u0436\u0435\u043d\u0438\u044f\u0020\u043d\u0430"
        L"\u0436\u043c\u0438\u0442\u0435\u0020\u043a\u043b\u0430\u0432\u0438"
        L"\u0448\u0443\u002e\u002e\u002e";
    sf::String desctext(sometext);
    sf::String maintext(L"2djourney");
    m_descText = new Text(desctext, font, 30);
    m_mainText = new Text(maintext, font, 59);

    sf::Vector2f windowSize = m_coreInstance->getWindowSize();
    m_bg->setSize(windowSize);
    m_bg->setFillColor(sf::Color(0, 0, 0, 150));
    
    sf::Vector2f textSize = m_descText->getGlobalBounds().getSize();
    m_descText->setOrigin(textSize / 2.f);
    m_descText->setPosition(windowSize.x / 2.f, windowSize.y * 0.75f);

    textSize = m_mainText->getGlobalBounds().getSize();
    m_mainText->setOrigin(textSize / 2.f);
    m_mainText->setPosition(windowSize.x / 2.f, windowSize.y * 0.35f);
}

PauseScreen::~PauseScreen() {
    delete m_bg;
    if (m_descText)
        delete m_descText;
    if (m_mainText)
        delete m_mainText;
}

namespace {
float textscale = 1.0;
bool isRising = true;
sf::Clock n_pauseWaitClock;
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
    if (n_pauseWaitClock.getElapsedTime().asMilliseconds() < 200)
        return;
    if (event.type == sf::Event::EventType::KeyPressed) {
        if (event.key.code == sf::Keyboard::R ||
                event.key.code == sf::Keyboard::P)
            return;
        if (m_isActive) {
            m_coreInstance->unsetPause();
            m_isActive = false;
        }
    }
}

void PauseScreen::setActive() {
    m_isActive = true;
    n_pauseWaitClock.restart();
}

void PauseScreen::setMainText(const sf::String &text) {
    if (m_mainText) {
        m_mainText->setString(text);
        auto textSize = m_mainText->getGlobalBounds().getSize();
        m_mainText->setOrigin(textSize / 2.f);
    }
}

void PauseScreen::draw(sf::RenderTarget &target, 
                        sf::RenderStates states) const {
    if (!m_isActive)
        return;
    target.draw(*m_bg);
    if (m_mainText)
        target.draw((sf::Text) *m_mainText, states);
    target.draw((sf::Text) *m_descText, states);
}

void PauseScreen::adjustScale(const sf::Vector2f &factors) {
    m_scale.x *= factors.x;
    m_scale.y *= factors.y;
}

PauseScreen::operator std::string() const {
    return "Object<PauseScreen>";
}
