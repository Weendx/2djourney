#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "object.h"


template <>
Object<sf::Sprite>::Object()
    : m_drawable(new sf::Sprite()), m_name("None") {}

template <>
Object<sf::Sprite>::~Object() {
    if (m_drawable != nullptr)
        delete m_drawable;
}


template <>
Object<sf::Text>::Object() {}

template <>
Object<sf::Text>::~Object() {
    if (m_drawable != nullptr)
        delete m_drawable;
}


template <class T>
Object<T>::Object():m_name("None") {}

template <class T>
Object<T>::Object(const T& drawable): m_drawable(&drawable) {}

template <typename T>
Object<T>::~Object() {
    if (m_drawable != nullptr)
        delete m_drawable;
}

template<>
void Object<sf::Text>::adjustScale(const sf::Vector2f &factors) {
    sf::Vector2f tmpScale(1.0, 1.0);
    tmpScale = m_drawable->getScale();
    tmpScale.x *= factors.x;
    tmpScale.y *= factors.y;
    m_drawable->setScale(tmpScale);
}

template<>
void Object<sf::Sprite>::adjustScale(const sf::Vector2f &factors) {
    sf::Vector2f tmpScale(1.0, 1.0);
    tmpScale = m_drawable->getScale();
    tmpScale.x *= factors.x;
    tmpScale.y *= factors.y;
    m_drawable->setScale(tmpScale);
}

