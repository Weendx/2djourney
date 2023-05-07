#pragma once

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include "actor.h"
#include "object.h"
#include "objects/debugInformer.h"
#include "objects/player.h"
#include "objects/map.h"

#include "box2d/b2_math.h"
#include "box2d/b2_world.h"

//class b2World;
//class b2Vec2;
class Core {
 public:
    Core();
    ~Core();
    void process();
    void handleEvent(sf::Event);
    void render();

    void registerActor(Actor* actor);
    void updateScale();

    float getFPS() const { return m_fps; }

    // const std::vector<Tile*> getTiles()const { return m_gameMap->getCollisionTiles(); }
    const Map* getMap() const { return m_gameMap; }
    DebugInformer* debug() const { return m_debugInformer; }
    b2World* getWorld() { return &m_world; }
    const b2World* getWorld() const { return &m_world; }  // ?
 private:
    void close();
    sf::RenderWindow* m_window;
    Map* m_gameMap;
    std::vector<Tile*> m_collisionTiles;
    DebugInformer* m_debugInformer;
    //std::vector<Object*> m_objects;
    std::vector<Actor*> m_actors;

    sf::Vector2f m_scale;
    sf::Vector2f m_screenSize;
    float m_fps;

    void setScale(const sf::Vector2f &newScale);
    void setScale(const float &factorX, const float &factorY);
    b2World m_world;
    b2Vec2 m_gravity{ 0.0, 10.0 };
};

