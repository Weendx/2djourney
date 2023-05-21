#include "core.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_shape.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_math.h>

#include <iostream>
#include <string>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/String.hpp>

#include "objects/background.h"
#include "objects/pauseScreen.h"
#include "objects/debugInformer.h"
#include "objects/player.h"
#include "objects/tile.h"
#include "b2DebugDraw.h"
#include "utils.h"
#include "generator.h"


namespace {
int n_mapShiftX = 0;
int n_isLevelCompleted = false;
}

Core::Core() 
    : m_scale(sf::Vector2f(1.0, 1.0)), m_fps(0.0),
        m_debugInformer(new DebugInformer()),
        m_screenSize(1280, 720), m_generator(new Generator()), 
        m_defaultFont(), m_mapSeed(1684599365) {
    m_world = new b2World(m_gravity);
    m_gameMap = new Map(this);
    m_gameMap->setBottom(m_screenSize.y);
    m_gameMap->setCoreInstance(this);

    if (m_showDebug) {
        m_gameMap->showDebug = true;
        m_debugInformer->showDebug = true;
        std::cout << "Map seed: " << m_mapSeed << std::endl;
    }

    this->generateMap();
    m_gameMap->init();
    this->generateMap();

    m_defaultFont = new sf::Font();
    if (!m_defaultFont->loadFromFile("resources/fonts/default.ttf"))
        throw std::runtime_error("Can't load default font");

    m_pauseScreen = new PauseScreen(this, *m_defaultFont);
    m_playTime = sf::Clock();
    m_background = new Background(this);

    m_b2DebugFlags = b2Draw::e_shapeBit;
    // m_b2DebugFlags += b2Draw::e_jointBit;
    // m_b2DebugFlags += b2Draw::e_pairBit;
    // m_b2DebugFlags += b2Draw::e_centerOfMassBit;
}

Core::~Core() {
    delete m_debugInformer;
    delete m_gameMap;
    delete m_background;
    delete m_pauseScreen;
    delete m_defaultFont;
    delete m_world;
}

void Core::close() {
    m_window->close();
    m_window = nullptr;  // Сейчас может и не надо, на будущее
}

void Core::process() {
    sf::RenderWindow window(
            sf::VideoMode(m_screenSize.x, m_screenSize.y), "2djourney");
    window.setVerticalSyncEnabled(true);
    m_window = &window;

    sf::Clock deltaClock;
    m_playTime.restart();

    b2DebugDraw b2dDraw(&window);
    b2dDraw.SetFlags(m_b2DebugFlags);
    m_world->SetDebugDraw(&b2dDraw);
    while ( window.isOpen() ) {
        sf::Time deltaTime = deltaClock.restart();
        sf::Event event;
        
        sf::View view;
        view.reset(sf::FloatRect(0, 0, m_screenSize.x, m_screenSize.y));
        
        // *** Event section *** //
        while (window.pollEvent(event)) {
            this->handleEvent(event);
            m_pauseScreen->handleEvent(event);
            for (auto e : m_objects)
                e->handleEvent(event);
        }

        // *** Update section *** //
        m_fps = 1 / deltaTime.asSeconds();
        m_debugInformer->setFPS(m_fps);

        // Game timer
        float elapsedTime = m_playTime.getElapsedTime().asSeconds();
        sf::String elapsed = secondsToString(elapsedTime);
        if (!m_isPaused) {
            m_debugInformer->updateString(L"\u0412\u0440\u0435\u043c\u044f"
                            L"\u0020\u0436\u0438\u0437\u043d\u0438", elapsed);
            m_debugInformer->updateString(L"\u041d\u0430\u0431\u0440\u0430"
                        L"\u043d\u043e\u0020\u043e\u0447\u043a\u043e\u0432", 
                        std::to_string(m_gameScore));
            if (n_isLevelCompleted) {
                this->nextLevel();
                n_isLevelCompleted = false;
            }
        }

        updateView(&view, playerCoords);
        window.setView(view);

        window.clear();


        m_debugInformer->onUpdate(deltaTime);
        m_pauseScreen->onUpdate(deltaTime);
        m_background->onUpdate(deltaTime);
        m_gameMap->onUpdate(deltaTime);

        if (!m_isPaused)
            m_world->Step(1.0f / 60, 8, 3);

        // *** Draw section *** //
        window.draw(*m_background);
        window.draw(*m_gameMap);
        for (auto e : m_objects) {
            e->onUpdate(deltaTime);  // Update registeredObjects
            window.draw(*e);
        }
        if (m_showDebug)
            m_world->DebugDraw();
        window.setView(window.getDefaultView());
        window.draw(*m_debugInformer);
        window.draw(*m_pauseScreen);

        window.display();
    }
}

void Core::handleEvent(sf::Event event) {
    if (event.type == sf::Event::Closed)
        this->close();
    if (event.type == sf::Event::EventType::KeyPressed) {
        if (event.key.code == sf::Keyboard::Q)
            this->close(); 
        if (event.key.code == sf::Keyboard::P)
            this->setPause();
        if (m_showDebug && event.key.code == sf::Keyboard::R)
            this->restartGame();
        if (m_showDebug && event.key.code == sf::Keyboard::G) {
            this->nextLevel();
            n_isLevelCompleted = false;
        }
        if (m_showDebug && event.key.code == sf::Keyboard::Y) {
            generateMap();
        }
    }
}

void Core::registerObject(Object* object) {
    m_objects.push_back(object);
    if (!object->hasPhysics())
        return;
    object->addPhysics(m_world);
}

void Core::setScale(const sf::Vector2f &newScale) {
    m_scale = newScale;
    updateScale();
}

void Core::setScale(const float &factorX, const float &factorY) {
    m_scale.x = factorX;
    m_scale.y = factorY;
    updateScale();
}

void Core::updateScale() {
    m_gameMap->adjustScale(m_scale);
    for (auto e : m_objects) 
        e->adjustScale(m_scale);
    m_debugInformer->adjustScale(m_scale);
}

sf::View* Core::updateView(sf::View* view, const sf::Vector2f& playerCoords) {
    view->setCenter(playerCoords);
    return view;
}

void Core::setPause(sf::String text) {
    m_isPaused = true;
    if (m_pauseScreen) {
        if (text.isEmpty())
            m_pauseScreen->setMainText(L"\u041f\u0430\u0443\u0437\u0430");
        else
            m_pauseScreen->setMainText(text);
        m_pauseScreen->setActive();
    }
}

void Core::unsetPause() {
    m_isPaused = false;
    // m_playTime.restart();
}

void Core::restartGame() {
    for (auto e : m_objects)
        e->onRestart();
    if (m_pauseScreen) {
        m_pauseScreen->setMainText("Game Over");
        m_pauseScreen->onRestart();
        m_isPaused = true;
    }
    m_playTime.restart();
}

void Core::nextLevel() {
    m_gameMap->deleteMap();
    this->generateMap(true);
    m_gameMap->init();
    this->generateMap();
    for (auto e : m_objects)
        e->onNextLevel();
}

void Core::changeScore(const int &delta) {
    if (m_gameScore + delta >= 0)
        m_gameScore += delta;
    else
        m_gameScore = 0;
}

void Core::generateMap(const bool& generateNew) {
    sf::Vector2f mapSize(60, 20);
    
    if (generateNew) {
        m_mapSeed = time(NULL);
        n_mapShiftX = 0;
        std::cout << "New map seed: " << m_mapSeed << std::endl;
    }

    GenResult gr = m_generator->randomWalkTop(
            mapSize.x, mapSize.y, m_mapSeed, n_mapShiftX, 3, 8);
    std::vector<std::string> gr_str = m_generator->to_string(gr);

    if (n_mapShiftX == 0) {
        m_gameMap->setMapLayout(gr_str);
    } else {
        m_gameMap->appendMapLayout(gr_str);
    }

    // #НеКостыль
    if (n_mapShiftX / mapSize.x > 3) {
        setPause(L"\u0423\u0440\u043e\u0432\u0435\u043d"
            L"\u044c\u0020\u043f\u0440\u043e\u0439\u0434\u0435\u043d\u0021");
        n_isLevelCompleted = true;
        m_gameScore += 100;
    }
    
    n_mapShiftX += mapSize.x;
}
