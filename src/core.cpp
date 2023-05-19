#include "core.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_shape.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_math.h>

#include <iostream>
#include <string>

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


Core::Core() 
    : m_scale(sf::Vector2f(1.0, 1.0)), m_fps(0.0),
        m_debugInformer(new DebugInformer()), m_gameMap(new Map()),
            m_screenSize(1280, 720), m_world(m_gravity), m_defaultFont() {
    m_world.SetGravity(m_gravity);
    m_gameMap->setBottom(m_screenSize.y);
    m_gameMap->setCoreInstance(this);
    if (m_showDebug) {
        m_gameMap->showDebug = false;
        m_debugInformer->showDebug = true;
    }
    m_gameMap->init();

    m_defaultFont = new sf::Font();
    if (!m_defaultFont->loadFromFile("resources/fonts/default.ttf"))
        throw std::runtime_error("Can't load default font");

    m_pauseScreen = new PauseScreen(this, *m_defaultFont);
    m_playTime = sf::Clock();
    m_background = new Background(this);

    m_b2DebugFlags += b2Draw::e_shapeBit;
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
    m_world.SetDebugDraw(&b2dDraw);
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
        if (!m_isPaused)
            m_debugInformer->updateString(L"Продолжительность игры", elapsed);

        updateView(&view, playerCoords);
        window.setView(view);

        window.clear();


        m_debugInformer->onUpdate(deltaTime);
        m_pauseScreen->onUpdate(deltaTime);
        m_background->onUpdate(deltaTime);

        if (!m_isPaused)
            m_world.Step(1.0f / 60, 8, 3);

        // *** Draw section *** //
        window.draw(*m_background);
        window.draw(*m_gameMap);
        for (auto e : m_objects) {
            e->onUpdate(deltaTime);
            window.draw(*e);
        }
        if (m_showDebug)
            m_world.DebugDraw();
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
        if (m_showDebug && event.key.code == sf::Keyboard::R)
            this->restartGame();
    }
    if (event.type == sf::Event::EventType::MouseButtonReleased) {
        sf::Vector2f mouseCoords(event.mouseButton.x, event.mouseButton.y);
        auto ttype = tileTypeToString(m_gameMap->getTileTypeAt(mouseCoords));
        m_debugInformer->updateDebugString("LastClickTileType", ttype);
    }
}

void Core::registerObject(Object* object) {
    m_objects.push_back(object);
    if (!object->hasPhysics())
        return;
    object->addPhysics(&m_world);
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

void Core::setPause() {
    m_isPaused = true;
}

void Core::unsetPause() {
    m_isPaused = false;
}

void Core::restartGame() {
    for (auto e : m_objects)
        e->onRestart();
    m_pauseScreen->onRestart();
    m_playTime.restart();
}
