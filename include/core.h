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

    void registerObject(Actor* actor);
    void registerObject(Object<sf::Text>* textObject);
    void setScale(const sf::Vector2f &newScale);
    void setScale(const float &factorX, const float &factorY);
    void updateScale();

    float fps;
 private:
    void close();
    sf::RenderWindow* m_window;
    std::vector<Actor*> m_actors;
    std::vector<Object<sf::Text>*> m_texts;
    sf::Vector2f m_scale;
};

#endif  // CORE_H
