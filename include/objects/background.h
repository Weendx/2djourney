#pragma once

#include <string>
#include "object.h"

class Core;
namespace sf {
class Sprite;
class Texture;
}


struct Bg {
    sf::Sprite *left, *center, *right;
};

class Background : virtual public Object {
 public:
    explicit Background(Core* coreInstance);
    virtual ~Background();
    void setPlayerPos(const sf::Vector2f& coord);

    void adjustScale(const sf::Vector2f &factors) override;
    operator std::string() const override;
    void onUpdate(const sf::Time &deltaTime) override;
    void onRestart() override;
    void handleEvent(const sf::Event &event) override;

 private:
    void draw(sf::RenderTarget& target, 
                    sf::RenderStates states) const override;
    void moveRight();
    void moveLeft();
    void setDefaultPosition();

    Bg m_bg;
    sf::Texture m_bgTexture;
    sf::Vector2f m_bgSize;
};
