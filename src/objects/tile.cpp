#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <SFML/System/Clock.hpp>
#include "objects/tile.h"

Tile::Tile(const TileType& type) 
    : m_type(type), sf::Sprite(), m_animClock(nullptr), m_animPhase(0) {
    loadTexture();
    if (!m_tileSheet)
        throw std::runtime_error("Tile textures aren't loaded");
    setTexture(*m_tileSheet);
    auto tileRect = getTileRect(type);
    setTextureRect(tileRect);
    m_class = getTileClass(type);
    // setOrigin(tileRect.width / 2.0, tileRect.height / 2.0);

    m_isCollide = type != TileType::Empty && type != Spikes && type != Coin;
    ++m_tilesCount;
    if (type == Coin) {
        m_animClock = new sf::Clock();
        m_frameTime = (std::rand() % 6) / 100.0 + 0.12;
        m_animPhase = std::rand() % 5;
    }
}

Tile::~Tile() {
    if (m_tilesCount == 1 && m_tileSheet) {
        delete m_tileSheet;
        m_tileSheet = nullptr;
        --m_tilesCount;
    }

    --m_tilesCount;
    if (m_animClock)
        delete m_animClock;
}

void Tile::adjustScale(const sf::Vector2f &factors) {
    sf::Vector2f tmpScale(1.0, 1.0);
    tmpScale = this->getScale();
    tmpScale.x *= factors.x;
    tmpScale.y *= factors.y;
    this->setScale(tmpScale);
}

void Tile::onUpdate(const sf::Time &deltaTime) {
    if (m_type != Coin)
        return;
    float dt = m_animClock->getElapsedTime().asSeconds();
    if (dt < m_frameTime)
        return;
    sf::IntRect spritePos = this->getTileRect(m_type);
    spritePos.left += 24 * m_animPhase;
    this->setTextureRect(spritePos);
    ++m_animPhase;
    m_animClock->restart();
    if (m_animPhase >= 5)  // 5 - animId
        m_animPhase = 0;
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw((sf::Sprite) *this, states);
}

void Tile::loadTexture() {
    if (m_tileSheet) return;
    m_tileSheet = new sf::Texture();
    if (!m_tileSheet->loadFromFile("resources/drawable/tileSheet.png"))
        throw std::runtime_error("Can't load tileSheet.png");
}

sf::Vector2f Tile::getHitboxSize() const {
    return getGlobalBounds().getSize();
}

const sf::IntRect Tile::getTileRect(const TileType& type) const {
    int8_t w = 24, h = 24;
    switch (type) {
        case GrassBegin:
            return sf::IntRect(0, 0, w, h);
        case Grass1:
            return sf::IntRect(24, 0, w, h);
        case Grass2:
            return sf::IntRect(48, 0, w, h);
        case GrassEnd:
            return sf::IntRect(72, 0, w, h);
        case GrassSingle:
            return sf::IntRect(240, 0, w, h);
        case GrassPlatformBegin:
            return sf::IntRect(120, 72, w, h);
        case GrassPlatformUp:
            return sf::IntRect(144, 48, w, h);
        case GrassPlatformEnd:
            return sf::IntRect(168, 72, w, h);
        case GrassPlatform1:
            return sf::IntRect(288, 192, w, h);
        case GrassPlatform2:
            return sf::IntRect(312, 192, w, h);
        case GrassSlopeUp1:
            return sf::IntRect(0, 193, w, h);
        case GrassSlopeUp2:
            return sf::IntRect(0, 169, w, h);
        case GrassSlopeUp3:
            return sf::IntRect(24, 169, w, h);
        case GrassSlopeDown3:
            return sf::IntRect(48, 169, w, h);
        case GrassSlopeDown2:
            return sf::IntRect(72, 169, w, h);
        case GrassSlopeDown1:
            return sf::IntRect(72, 193, w, h);
        case StoneToGrass:
            return sf::IntRect(288, 145, w, h);
        case GrassToStone:
            return sf::IntRect(312, 145, w, h);
        case SideLeft1:
            return sf::IntRect(0, 24, w, h);
        case SideLeft2:
            return sf::IntRect(0, 48, w, h);
        case SideLeft3:
            return sf::IntRect(0, 72, w, h);
        case SideRight1:
            return sf::IntRect(72, 24, w, h);
        case SideRight2:
            return sf::IntRect(72, 48, w, h);
        case SideRight3:
            return sf::IntRect(72, 72, w, h);
        case GroundNStone1:
            return sf::IntRect(24, 72, w, h);
        case GroundNStone2:
            return sf::IntRect(48, 72, w, h);
        case StoneBegin:
            return sf::IntRect(120, 0, w, h);
        case Stone1:
            return sf::IntRect(144, 0, w, h);
        case Stone2:
            return sf::IntRect(168, 0, w, h);
        case StoneEnd:
            return sf::IntRect(192, 0, w, h);
        case StoneSingle:
            return sf::IntRect(288, 0, w, h);
        case StonePlatformBegin:
            return sf::IntRect(216, 72, w, h);
        case StonePlatformUp:
            return sf::IntRect(240, 48, w, h);
        case StonePlatformEnd:
            return sf::IntRect(264, 72, w, h);
        case StonePlatform1:
            return sf::IntRect(288, 240, w, h);
        case StonePlatform2:
            return sf::IntRect(312, 240, w, h);
        case Spikes:
            return sf::IntRect(24, 48, w, h);
        case StoneSlopeUp2:
            return sf::IntRect(0, 120, w, h);
        case StoneSlopeUp3:
            return sf::IntRect(24, 120, w, h);
        case StoneSlopeDown3:
            return sf::IntRect(48, 120, w, h);
        case StoneSlopeDown2:
            return sf::IntRect(72, 120, w, h);
        case Coin:
            return sf::IntRect(216, 276, w, h);
        case VertStone1:
            return sf::IntRect(144, 120, w, h);
        case VertStone2:
            return sf::IntRect(144, 96, w, h);
        case VertStone3:
            return sf::IntRect(240, 96, w, h);
        case Ground:
            return sf::IntRect(24, 24, w, h);
        case Empty:
            return sf::IntRect(48, 24, w, h);
    }
    throw std::invalid_argument("Invalid tile type");
}

TileClass Tile::getTileClass(const TileType& type) const {
    switch (type) {
        case GrassBegin:
        case Grass1:
        case Grass2:
        case GrassEnd:
        case GrassSingle:
        case GrassPlatformBegin:
        case GrassPlatformUp:
        case GrassPlatformEnd:
        case GrassPlatform1:
        case GrassPlatform2:
        case GrassSlopeUp1:
        case GrassSlopeUp2:
        case GrassSlopeUp3:
        case GrassSlopeDown3:
        case GrassSlopeDown2:
        case GrassSlopeDown1:
            return Grass;
        case StoneToGrass:
            return Stone;
        case GrassToStone:
            return Grass;
        case SideLeft1:
        case SideLeft2:
        case SideLeft3:
        case SideRight1:
        case SideRight2:
        case SideRight3:
        case GroundNStone1:
        case GroundNStone2:
            return Wall;
        case StoneBegin:
        case Stone1:
        case Stone2:
        case StoneEnd:
        case StoneSingle:
        case StonePlatformBegin:
        case StonePlatformUp:
        case StonePlatformEnd:
        case StonePlatform1:
        case StonePlatform2:
            return Stone;
        case Spikes:
            return Spike;
        case StoneSlopeUp2:
        case StoneSlopeUp3:
        case StoneSlopeDown3:
        case StoneSlopeDown2:
            return Stone;
        case Coin:
            return Reward;
        case VertStone1:
        case VertStone2:
        case VertStone3:
            return Wall;
        case Ground:
            return Soil;
        case Empty:
            return Void;
    }
    throw std::invalid_argument("Tile::getTileClass(): Invalid tile type");
}

Tile::operator std::string() const { 
    return "Tile <" + tileTypeToString(m_type) + ", width: " +
        std::to_string(getGlobalBounds().width) + 
        " , height: " + std::to_string(getGlobalBounds().height) + ">"; 
}

std::string tileTypeToString(const TileType& type) {
    switch (type) {
        case GrassBegin:
            return "GrassBegin";
        case Grass1:
            return "Grass1";
        case Grass2:
            return "Grass2";
        case GrassEnd:
            return "GrassEnd";
        case GrassSingle:
            return "GrassSingle";
        case GrassPlatformBegin:
            return "GrassPlatformBegin";
        case GrassPlatformUp:
            return "GrassPlatformUp";
        case GrassPlatformEnd:
            return "GrassPlatformEnd";
        case GrassPlatform1:
            return "GrassPlatform1";
        case GrassPlatform2:
            return "GrassPlatform2";
        case GrassSlopeUp1:
            return "GrassSlopeUp1";
        case GrassSlopeUp2:
            return "GrassSlopeUp2";
        case GrassSlopeUp3:
            return "GrassSlopeUp3";
        case GrassSlopeDown3:
            return "GrassSlopeDown3";
        case GrassSlopeDown2:
            return "GrassSlopeDown2";
        case GrassSlopeDown1:
            return "GrassSlopeDown1";
        case StoneToGrass:
            return "StoneToGrass";
        case GrassToStone:
            return "GrassToStone";
        case SideLeft1:
            return "SideLeft1";
        case SideLeft2:
            return "SideLeft2";
        case SideLeft3:
            return "SideLeft3";
        case SideRight1:
            return "SideRight1";
        case SideRight2:
            return "SideRight2";
        case SideRight3:
            return "SideRight3";
        case GroundNStone1:
            return "GroundNStone1";
        case GroundNStone2:
            return "GroundNStone2";
        case StoneBegin:
            return "StoneBegin";
        case Stone1:
            return "Stone1";
        case Stone2:
            return "Stone2";
        case StoneEnd:
            return "StoneEnd";
        case StoneSingle:
            return "StoneSingle";
        case StonePlatformBegin:
            return "StonePlatformBegin";
        case StonePlatformUp:
            return "StonePlatformUp";
        case StonePlatformEnd:
            return "StonePlatformEnd";
        case StonePlatform1:
            return "StonePlatform1";
        case StonePlatform2:
            return "StonePlatform2";
        case Spikes:
            return "Spikes";
        case StoneSlopeUp2:
            return "StoneSlopeUp2";
        case StoneSlopeUp3:
            return "StoneSlopeUp3";
        case StoneSlopeDown3:
            return "StoneSlopeDown3";
        case StoneSlopeDown2:
            return "StoneSlopeDown2";
        case Coin:
            return "Coin";
        case VertStone1:
            return "VertStone1";
        case VertStone2:
            return "VertStone2";
        case VertStone3:
            return "VertStone3";
        case Ground:
            return "Ground";
        case Empty:
            return "Empty";
        default:
            return "";
    }
}

