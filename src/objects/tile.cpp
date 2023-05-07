#include <cstdint>
#include <stdexcept>
#include <string>
#include "objects/tile.h"

Tile::Tile(const TileType& type) 
    : m_type(type), m_isCollide(true), sf::Sprite() {
    loadTexture();
    if (!m_tileSheet)
        throw std::runtime_error("Tile textures aren't loaded");
    setTexture(*m_tileSheet);
    setTextureRect(getTileRect(type));
}

Tile::~Tile() {
    if (m_tileSheet)
        delete m_tileSheet;
}

void Tile::adjustScale(const sf::Vector2f &factors) {
    sf::Vector2f tmpScale(1.0, 1.0);
    tmpScale = this->getScale();
    tmpScale.x *= factors.x;
    tmpScale.y *= factors.y;
    this->setScale(tmpScale);
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

const sf::IntRect Tile::getTileRect(const TileType& type) {
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
        case StoneSlopeUp1:
            return sf::IntRect(0, 144, w, h);
        case StoneSlopeUp2:
            return sf::IntRect(0, 120, w, h);
        case StoneSlopeUp3:
            return sf::IntRect(24, 120, w, h);
        case StoneSlopeDown3:
            return sf::IntRect(48, 120, w, h);
        case StoneSlopeDown2:
            return sf::IntRect(72, 120, w, h);
        case StoneSlopeDown1:
            return sf::IntRect(72, 144, w, h);
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
        case StoneSlopeUp1:
            return "StoneSlopeUp1";
        case StoneSlopeUp2:
            return "StoneSlopeUp2";
        case StoneSlopeUp3:
            return "StoneSlopeUp3";
        case StoneSlopeDown3:
            return "StoneSlopeDown3";
        case StoneSlopeDown2:
            return "StoneSlopeDown2";
        case StoneSlopeDown1:
            return "StoneSlopeDown1";
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

