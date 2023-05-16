#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_shape.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_math.h"

#include "core.h"
#include "objects/player.h"
#include "objects/tile.h"
#include "text.h"
#include "objects/debugInformer.h"
#include "b2DebugDraw.h"
#include "utils.h"


Core::Core() 
    : m_scale(sf::Vector2f(1.0, 1.0)), m_fps(0.0),
        m_debugInformer(new DebugInformer()), m_gameMap(new Map()),
                                            m_screenSize(1280, 720), m_world(m_gravity) {
    m_world.SetGravity(m_gravity);
    m_gameMap->setBottom(m_screenSize.y);
    m_gameMap->setCoreInstance(this);
    if (m_showDebug) {
        m_gameMap->showDebug = false;
        m_debugInformer->showDebug = true;
    }
    m_gameMap->init();


    m_b2DebugFlags += b2Draw::e_shapeBit;
    // m_b2DebugFlags += b2Draw::e_jointBit;
    //m_b2DebugFlags += b2Draw::e_pairBit;
    // m_b2DebugFlags += b2Draw::e_centerOfMassBit;
}

Core::~Core() {
    delete m_debugInformer;
}

void Core::close() {
    m_window->close();
    m_window = nullptr;  // Сейчас может и не надо, на будущее
}

void Core::process() {
    sf::Texture background;
    background.loadFromFile("resources/drawable/bg.png");
    sf::Sprite bg;
    bg.setTexture(background);
    bg.setPosition(0.0, -500.0);

    sf::RenderWindow window(
            sf::VideoMode(m_screenSize.x, m_screenSize.y), "2djourney");
    window.setVerticalSyncEnabled(true);
    m_window = &window;

    sf::Clock deltaClock;

    b2DebugDraw b2dDraw(&window);
    b2dDraw.SetFlags(m_b2DebugFlags);
    m_world.SetDebugDraw(&b2dDraw);
    while ( window.isOpen() ) {
        sf::Time deltaTime = deltaClock.restart();
        sf::Event event;
        
        sf::View view;
        view.reset(sf::FloatRect(0, 0, 1280, 720));
        
        while (window.pollEvent(event)) {
            this->handleEvent(event);
            for (auto e : m_objects)
                e->handleEvent(event);
        }
        m_fps = 1 / deltaTime.asSeconds();
        m_debugInformer->setFPS(m_fps);

        updateView(view, playerCoords);
        window.setView(view);

        window.clear();
        window.draw(bg);

        window.draw(*m_gameMap);

        m_debugInformer->onUpdate(deltaTime);

        m_world.Step((float) 1 / 60, 8, 3);

        for (auto e : m_objects) {
            e->onUpdate(deltaTime);
            window.draw(*e);
        }
        if (m_showDebug)
            m_world.DebugDraw();
        window.setView(window.getDefaultView());
        window.draw(*m_debugInformer);

        window.display();
    }
}

void Core::handleEvent(sf::Event event) {
    if (event.type == sf::Event::Closed)
        this->close();
    if (event.type == sf::Event::EventType::KeyPressed) {
        if (event.key.code == sf::Keyboard::Q)
            this->close(); 
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

sf::View Core::updateView(sf::View& view, sf::Vector2f& playerCoords) {
    view.setCenter(playerCoords);
    return view;
}
