#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "core.h"
#include "objects/tile.h"
#include "text.h"

#include "objects/debugInformer.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_shape.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_math.h"

Core::Core() 
    : m_scale(sf::Vector2f(1.0, 1.0)), m_fps(0.0),
        m_debugInformer(new DebugInformer()), m_gameMap(new Map()),
                                            m_screenSize(1280, 720), m_world(m_gravity) {
    m_gameMap->setBottom(m_screenSize.y);
    m_gameMap->createTiles();
    m_gameMap->setCoreInstance(this);
}

Core::~Core() {
    delete m_debugInformer;
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
    
    while ( window.isOpen() ) {
        sf::Time deltaTime = deltaClock.restart();
        sf::Event event;

        while (window.pollEvent(event)) {
            this->handleEvent(event);
            for (auto e : m_actors)
                e->handleEvent(event);
        }
        
        m_fps = 1 / deltaTime.asSeconds();
        m_debugInformer->setFPS(m_fps);

        window.clear();

        window.draw(*m_gameMap);

        m_debugInformer->onUpdate(deltaTime);
        window.draw(*m_debugInformer);

        m_world.Step((float) 1 / 60, 8, 3);

        for (auto e : m_actors) {
            e->onUpdate(deltaTime);
            window.draw((sf::Sprite)*e);
        }

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

void Core::registerActor(Actor* actor) {
    m_actors.push_back(actor);
    if (!actor->hasPhysics())
        return;
    b2BodyDef bdef;
    if (actor->isItDynamic())
        bdef.type = b2_dynamicBody;
    bdef.position.Set(actor->getGlobalBounds().left/24, actor->getGlobalBounds().top/24);

    b2Body* body = m_world.CreateBody(&bdef);

    b2PolygonShape ps;
    sf::Vector2f hitboxsizes = actor->getHitBoxSize();
    ps.SetAsBox(hitboxsizes.x/2/24, hitboxsizes.y/2/24);


    b2FixtureDef fd;
    fd.shape = &ps;

    fd.density = 1;
    fd.friction = 0;
    fd.restitution = 0;

    body->CreateFixture(&fd);
    actor->setBody(body);
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
    for (auto e : m_actors) 
        e->adjustScale(m_scale);
    m_debugInformer->adjustScale(m_scale);
}
