#include <iostream>
#include <stdexcept>
#include <SFML/Graphics.hpp>


int main(int argc, char* argv[]) {
    sf::RenderWindow window(sf::VideoMode(856, 482), "Test run");
    window.setVerticalSyncEnabled(true);
    
    sf::Font defaultFont;
    if (!defaultFont.loadFromFile("src/resources/fonts/default.ttf"))
        throw std::runtime_error("Can't load default font");

    float fps;
    // sf::Clock generalClock; // for global elapsed time
    sf::Clock deltaClock; // for delta time
    sf::Clock fpsUpdateTimer;
    sf::Text fpsCounter("FPS: 0", defaultFont, 15);

    sf::CircleShape shape(100.0);
    shape.setFillColor(sf::Color::Red);

    while (window.isOpen()) {
        float deltaTime = deltaClock.restart().asSeconds();
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        fps = 1 / deltaTime;
        if (fpsUpdateTimer.getElapsedTime().asSeconds() >= 0.5) {
            fpsCounter.setString("FPS: " + std::to_string((short) fps));
            fpsCounter.setFillColor(sf::Color::White);
            fpsUpdateTimer.restart();
        }

        window.clear();
        window.draw(shape);
        window.draw(fpsCounter);
        window.display();
    }
    return 0;
}
