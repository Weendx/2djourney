#ifndef CORE_H
#define CORE_H

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include "actor.h"
#include "object.h"

class Core {
 public:
    Core();
    void process();
    void handleEvent(sf::Event);
    void render();

    // void registerObject(Actor* actor);
    void registerObject(Object* object);
    void setScale(const sf::Vector2f &newScale);
    void setScale(const float &factorX, const float &factorY);
    void updateScale();
   
    float getFPS() const { return m_fps; }
 private:
    void close();
    sf::RenderWindow* m_window;
    std::vector<Object*> m_objects;
    // std::vector<Object*> m_actors;
    // std::vector<Object*> m_texts;
    sf::Vector2f m_scale;
    float m_fps;
};

#endif  // CORE_H
