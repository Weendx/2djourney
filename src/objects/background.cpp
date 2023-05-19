#include "objects/background.h"
#include <cmath>
#include <stdexcept>
#include <string>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "core.h"


Background::Background(Core* coreInstance) : m_bgTexture() {
    // Set backgound size
    m_bgSize = {1738, 1272};

    setCoreInstance(coreInstance);
    if (!m_bgTexture.loadFromFile("resources/drawable/bg.png"))
        throw std::runtime_error("Can't load bg.png file");

    const int bgCount = 3;
    sf::Sprite* bg[bgCount];
    for (int i = 0; i < bgCount; ++i) {
        bg[i] = new sf::Sprite(m_bgTexture, 
                sf::IntRect(0, 0, m_bgSize.x, m_bgSize.y));
    }

    m_bg.right = bg[0];
    m_bg.center = bg[1];
    m_bg.left = bg[2];

    setDefaultPosition();
}

Background::~Background() {
    delete m_bg.right;
    delete m_bg.center;
    delete m_bg.left;
}

void Background::adjustScale(const sf::Vector2f &factors) {
    sf::Vector2f newScale = m_bg.center->getScale();
    newScale.x *= factors.x;
    newScale.y *= factors.y;
    m_bgSize.x *= factors.x;
    m_bgSize.y *= factors.y;
    m_bg.left->setScale(newScale);
    m_bg.center->setScale(newScale);
    m_bg.right->setScale(newScale);
}

Background::operator std::string() const {
    return "Object<Background>";
}

void Background::onUpdate(const sf::Time &deltaTime) {
    sf::Vector2f player = m_coreInstance->getPlayerCoords();
    float centerX = m_bg.center->getPosition().x + m_bgSize.x / 2;
    
    m_coreInstance->debug()->updateDebugString(
            "Dist to m_bg.center", std::to_string(player.x - centerX));
    if (std::abs(player.x - centerX) < m_bgSize.x)
        return;

    if (player.x - centerX > 0)
        moveRight();
    else
        moveLeft();
}

void Background::onRestart() {
    setDefaultPosition();
}

void Background::handleEvent(const sf::Event &event) {}

void Background::draw(sf::RenderTarget& target, 
                        sf::RenderStates states) const {
    target.draw(*m_bg.left, states);
    target.draw(*m_bg.center, states);
    target.draw(*m_bg.right, states);
}

void Background::moveRight() {
    sf::Vector2f newPos = m_bg.right->getPosition();
    newPos.x += m_bgSize.x;
    m_bg.left->setPosition(newPos);
    
    sf::Sprite* right = m_bg.right;
    sf::Sprite* center = m_bg.center;
    sf::Sprite* left = m_bg.left;

    m_bg.left = center;
    m_bg.center = right;
    m_bg.right = left;
}

void Background::moveLeft() {
    sf::Vector2f newPos = m_bg.left->getPosition();
    newPos.x -= m_bgSize.x;
    m_bg.right->setPosition(newPos);

    sf::Sprite* right = m_bg.right;
    sf::Sprite* center = m_bg.center;
    sf::Sprite* left = m_bg.left;

    m_bg.left = right;
    m_bg.center = left;
    m_bg.right = center;
}

void Background::setDefaultPosition() {
    int bgStartX = 0;
    int bgStartY = -500;
    m_bg.left->setPosition(bgStartX - m_bgSize.x, bgStartY);
    m_bg.center->setPosition(bgStartX, bgStartY);
    m_bg.right->setPosition(bgStartX + m_bgSize.x, bgStartY);
}
