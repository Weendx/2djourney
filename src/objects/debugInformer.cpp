#include "objects/debugInformer.h"
#include "text.h"

DebugInformer::DebugInformer() : m_fps(0.0), m_updateTimer() {
    m_font = new sf::Font();
    if (!(*m_font).loadFromFile("resources/fonts/default.ttf"))
        throw std::runtime_error("Can't load default font");
    m_fpsCounter = new FpsCounter(*m_font, m_textSize);
    m_debugTextBlock = new Text("", *m_font, m_textSize);
    m_userTextBlock = new Text("", *m_font, m_textSize);

    m_fpsCounter->setPosition(m_anchorPos);
    m_textAnchor.x = m_anchorPos.x;
    m_textAnchor.y = m_anchorPos.y + m_textSize + 5;

    m_debugTextBlock->setPosition(m_textAnchor);
    m_userTextBlock->setPosition(m_textAnchor);
}

DebugInformer::~DebugInformer() {
    delete m_fpsCounter;
    delete m_font;
}

void DebugInformer::onUpdate(const sf::Time& deltaTime) {
    if (m_updateTimer.getElapsedTime().asSeconds() < 0.5)
        return;
    m_fpsCounter->setFps(m_fps);
    m_updateTimer.restart();
}

void DebugInformer::handleEvent(const sf::Event& event) {}

void DebugInformer::adjustScale(const sf::Vector2f &factors) {
    m_fpsCounter->adjustScale(factors);
}

void DebugInformer::draw(sf::RenderTarget& target, 
        sf::RenderStates states) const {
    target.draw((sf::Text) *m_fpsCounter, states);
    target.draw((sf::Text) *m_userTextBlock, states);
    if (!showDebug)
        return;
    target.draw((sf::Text) *m_debugTextBlock, states);
}

void DebugInformer::updateTextStrings() {
    m_debugTextString.clear();
    m_userTextString.clear();
    for (const auto& [key, value] : m_additionInfo) {
        m_debugTextString += key + ": " + value + '\n';
    }
    for (const auto& [key, value] : m_userInfo) {
        m_userTextString += key + ": " + value + '\n';
    }
}

void DebugInformer::setFPS(const float& fps) {
    m_fps = fps;
}

void DebugInformer::addDebugString(
        const std::string& key, const sf::String& text) {
    m_additionInfo[key] = text;
    updateTextStrings();
    m_debugTextBlock->setString(m_debugTextString);
}

void DebugInformer::updateDebugString(
        const std::string& key, const sf::String& text) {
    addDebugString(key, text);
}

void DebugInformer::updateString(
        const sf::String &key, const sf::String &text) {
    m_userInfo[key] = text;
    updateTextStrings();
    m_userTextBlock->setString(m_userTextString);
    adjustDebugBlock();
}

void DebugInformer::adjustDebugBlock() {
    auto userTSize = m_userTextBlock->getGlobalBounds().getSize();
    m_debugTextBlock->setPosition(m_textAnchor.x, 
            m_textAnchor.y + userTSize.y + 5);
}

