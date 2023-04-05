#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "core.h"
#include "text.h"

#include "objects/debugInformer.h"

Core::Core() 
    : m_scale(sf::Vector2f(1.0, 1.0)), m_fps(0.0) {}

void Core::close() {
    m_window->close();
    m_window = nullptr;  // Сейчас может и не надо, на будущее
}

void Core::process() {
    sf::RenderWindow window(sf::VideoMode(856, 482), "2djourney");
    window.setVerticalSyncEnabled(true);
    m_window = &window;

    sf::Clock deltaClock;
    

    /// TODO: вынести
    
    DebugInformer* debugInformer = new DebugInformer();
    registerObject(debugInformer);

    /// ??
    
    updateScale();
    
    while ( window.isOpen() ) {
        sf::Time deltaTime = deltaClock.restart();
        sf::Event event;


        while (window.pollEvent(event)) {
            this->handleEvent(event);
            for (auto e : m_objects)
                e->handleEvent(event);
        }
        
        // TODO: вынести
        m_fps = 1 / deltaTime.asSeconds();
        debugInformer->setFPS(m_fps);
        // if (fpsUpdateTimer.getElapsedTime().asSeconds() >= 0.5) {
        //     fpsCounter->setString("FPS: " + std::to_string((int8_t) fps));
        //     fpsCounter->setFillColor(sf::Color::White);
        //     fpsUpdateTimer.restart();
        // }

        window.clear();

        for (auto e : m_objects) {
            e->onUpdate(deltaTime);
            window.draw(*e);
        }
        //
        // for (auto e : m_texts)
        //     window.draw(*e);

        window.display();
    }

    delete debugInformer;
}

void Core::handleEvent(sf::Event event) {
    if (event.type == sf::Event::Closed)
        this->close();
    if (event.type == sf::Event::EventType::KeyPressed) {
        if (event.key.code == sf::Keyboard::Q)
            this->close(); 
    }
}

// void Core::registerObject(Actor* actor) {
//     m_actors.push_back(actor);    
// }

void Core::registerObject(Object* object) {
    m_objects.push_back(object);
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
    for (auto e : m_objects) 
        e->adjustScale(m_scale);
    // for (auto e : m_texts)
    //     e->adjustScale(m_scale);
}
