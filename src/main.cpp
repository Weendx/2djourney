#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

// marks:
// https://itecnote.com/tecnote/c-shapes-proportionally-resize-with-window-in-sfml-2-x/

int main(int argc, char* argv[]) {
    sf::RenderWindow window(sf::VideoMode(856, 482), "2djourney");
    window.setVerticalSyncEnabled(true);
    
    sf::Font defaultFont;
    if (!defaultFont.loadFromFile("src/resources/fonts/default.ttf"))
        throw std::runtime_error("Can't load default font");

    float fps;
    // sf::Clock generalClock; // for global elapsed time
    sf::Clock deltaClock;  // for delta time
    sf::Clock fpsUpdateTimer;
    sf::Text fpsCounter("FPS: 0", defaultFont, 15);

    sf::CircleShape shape(100.0);
    shape.setFillColor(sf::Color::Red);

    // *** player sprite
    float scaleFactor = 1.6;
    sf::Texture playerSheet;
    if (!playerSheet.loadFromFile("src/resources/drawable/playerSheet.png"))
        throw std::runtime_error("Can't load player sprites");
    sf::Sprite playerIdle1(playerSheet, sf::IntRect(0, 0, 50, 37));
    playerIdle1.setScale(scaleFactor, scaleFactor);
    playerIdle1.setOrigin(50 / 2.0, 37 / 2.0);
    sf::Vector2<float> playerPos(30, 80);
    playerIdle1.setPosition(playerPos);
    // ***

    while (window.isOpen()) {
        sf::Time deltaTime = deltaClock.restart();
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                    window.close();
                
                //
                float dx = 0, dy = 0, speed = 0.4;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                    dx = speed;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                    dx = -speed;
                }
                std::cout << "PlayerPos: " << playerPos.x << ", " << playerPos.y << std::endl;  
                playerPos.x += dx * speed * deltaTime.asMilliseconds();
                playerIdle1.setPosition(playerPos);
            }
        }

        fps = 1 / deltaTime.asSeconds();
        if (fpsUpdateTimer.getElapsedTime().asSeconds() >= 0.5) {
            fpsCounter.setString("FPS: " + std::to_string((int8_t) fps));
            fpsCounter.setFillColor(sf::Color::White);
            fpsUpdateTimer.restart();
        }

        window.clear();
        // window.draw(shape);
        window.draw(playerIdle1);
        window.draw(fpsCounter);
        window.display();
    }
    return 0;
}
