#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "SFML/System/Time.hpp"
#include "SFML/Window/Event.hpp"

class Object: public sf::Drawable {
 public:
    Object() {}
    ~Object() {}
    void setName(const std::string &name) { m_name = name; }
    virtual void adjustScale(const sf::Vector2f &factors) = 0;
    virtual operator std::string() const = 0;
    virtual void onUpdate(const sf::Time &deltaTime) = 0;
    virtual void handleEvent(const sf::Event &event) = 0;
 protected:
    std::string m_name;
};

#endif  // OBJECT_H
