#pragma once

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/View.hpp>

#include "actor.h"
#include "object.h"
#include "objects/debugInformer.h"
#include "objects/player.h"
#include "objects/map.h"

#include "box2d/b2_math.h"
#include "box2d/b2_world.h"


class PauseScreen;
class Background;

class Core {
 public:
    Core();
    ~Core();
    void process();
    void handleEvent(sf::Event);
    void render();

    void registerObject(Object* object);
    void updateScale();
    void setPlayerCoords(const sf::Vector2f& coords) { playerCoords = coords; }
    sf::Vector2f getPlayerCoords() { return playerCoords; }
    sf::View* updateView(sf::View* view, const sf::Vector2f& playerCoords);

    float getFPS() const { return m_fps; }
    sf::Vector2f getWindowSize() const { return m_screenSize; }

    const Map* getMap() const { return m_gameMap; }
    DebugInformer* debug() const { return m_debugInformer; }
    b2World* getWorld() { return &m_world; }
    const b2World* getWorld() const { return &m_world; }  // ?

    void setPause();
    void unsetPause();
    void restartGame();

 private:
    void close();
    DebugInformer* m_debugInformer;
    sf::RenderWindow* m_window;
    PauseScreen* m_pauseScreen;
    Background* m_background;
    sf::Font* m_defaultFont;
    Map* m_gameMap;

    std::vector<Object*> m_objects;

    sf::Vector2f m_scale;
    sf::Vector2f m_screenSize;
    float m_fps;

    void setScale(const sf::Vector2f &newScale);
    void setScale(const float &factorX, const float &factorY);
    b2World m_world;
    b2Vec2 m_gravity{ 0.0f, 10.0f };

    sf::Vector2f playerCoords;

    uint32 m_b2DebugFlags;

    bool m_showDebug = true;
    bool m_isPaused = true;
    
    sf::Sprite* m_bg[3];
    sf::Clock m_playTime;
};

