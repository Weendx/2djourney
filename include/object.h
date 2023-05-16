#pragma once

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

class b2World;
class Core;
class b2Body;
class Object: public sf::Drawable {
 public:
    Object() {}
    ~Object() {}
    void setName(const std::string &name) { m_name = name; }
    virtual void adjustScale(const sf::Vector2f &factors) = 0;
    virtual operator std::string() const = 0;
    virtual void onUpdate(const sf::Time &deltaTime) = 0;
    virtual void handleEvent(const sf::Event &event) = 0;
    virtual sf::Vector2f getHitBoxSize();
    virtual void addPhysics(b2World* world) {}
    void setCoreInstance(Core* core) { m_coreInstance = core; }
    const bool hasPhysics() const { return m_hasPhysics; }
    const bool isItDynamic() const { return m_isDynamic; }
    void setBody(b2Body* body) { m_body = body; }
    b2Body* getBody() const { return m_body; }
    bool showDebug = false;
 protected:
    std::string m_name;
    Core* m_coreInstance = nullptr;
    b2Body* m_body = nullptr;
    bool m_hasPhysics;
    bool m_isDynamic;
};
