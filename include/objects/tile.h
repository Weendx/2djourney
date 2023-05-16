#pragma once
#include <cstdint>
#include <string>
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "object.h"


enum TileType {
    GrassBegin = 'A', Grass1 = 'B', Grass2 = 'C', GrassEnd = 'D', 
    GrassSingle = 'E', GrassPlatformBegin = 'F', GrassPlatformUp = 'G',
    GrassPlatformEnd = 'H', GrassPlatform1 = 'I', GrassPlatform2 = 'J',
    GrassSlopeUp1 = 'K', GrassSlopeUp2 = 'L', GrassSlopeUp3 = 'M',
    GrassSlopeDown3 = 'N', GrassSlopeDown2 = 'O', GrassSlopeDown1 = 'P',

    StoneToGrass = 'Q', GrassToStone = 'R',
    SideLeft1 = 'S', SideLeft2 = 'T', SideLeft3 = 'U',
    SideRight1 = 'V', SideRight2 = 'W', SideRight3 = 'X',
    GroundNStone1 = 'Y', GroundNStone2 = 'Z',

    StoneBegin = 'a', Stone1 = 'b', Stone2 = 'c', StoneEnd = 'd',
    StoneSingle = 'e', StonePlatformBegin = 'f', StonePlatformUp = 'g',
    StonePlatformEnd = 'h', StonePlatform1 = 'i', StonePlatform2 = 'j',
    StoneSlopeUp1 = 'k', StoneSlopeUp2 = 'l', StoneSlopeUp3 = 'm',
    StoneSlopeDown3 = 'n', StoneSlopeDown2 = 'o', StoneSlopeDown1 = 'p',

    VertStone1 = 'q', VertStone2 = 'r', VertStone3 = 's',

    Ground = '1', Empty = '0'
};

std::string tileTypeToString(const TileType& type);

class Tile : public Object, public sf::Sprite {
 public:
    explicit Tile(const TileType& type);
    virtual ~Tile();
    void adjustScale(const sf::Vector2f &factors) override;
    void onUpdate(const sf::Time &deltaTime) override {}
    void handleEvent(const sf::Event &event) override {}
    operator std::string() const override;
    TileType getType() { return m_type; }
    sf::Vector2f getHitboxSize() const;
    const bool& hasCollision() const { return m_isCollide; }
 private:
    TileType m_type;
    static inline sf::Texture* m_tileSheet;
    static inline uint32_t m_tilesCount = 0;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void loadTexture();
    const sf::IntRect getTileRect(const TileType& type);
    bool m_isCollide;
};

