#include "objects/map.h"
#include "objects/tile.h"
#include <iostream>
#include <stdexcept>

Map::Map() {
    fillMap();
    createTiles();
}

Map::Map(const int& bottom) : m_bottom(bottom) {
    fillMap();
    createTiles();
}

Map::~Map() {
    for (auto layer: m_layers)
        for (auto tile: layer)
            delete tile;
}

void Map::adjustScale(const sf::Vector2f &factors) {
    for (auto layer: m_layers)
        for (auto tile: layer)
            tile->adjustScale(factors);
}

void Map::fillMap() {
    m_currentMapLayout.push_back("UYZYZYZYZYZYZYZYZYZX");
    m_currentMapLayout.push_back("T111111111111111111W");
    m_currentMapLayout.push_back("S111111111111111111V");
    m_currentMapLayout.push_back("T111111111111111111W");
    m_currentMapLayout.push_back("S111111111111111111V");
    m_currentMapLayout.push_back("T11111111PCBCK11111W");
    m_currentMapLayout.push_back("ABCBCBCBNO000LMBCBCD");
}

void Map::createTiles() {
    for (auto layer: m_currentMapLayout) {
        std::vector<Tile*> layerTiles;
        for (auto tileName: layer) {
            layerTiles.push_back(new Tile( (TileType)tileName ));
            layerTiles.back()->adjustScale(m_scale);
        }
        m_layers.push_back(layerTiles);
    }
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // TODO: вынести расчеты
    float lengthY = m_bottom;
    bool isTileHeightChanged = false;
    float tileHeight = 0;
    for (auto layer: m_layers) {
        float lengthX = 0;
        for (auto tile: layer) {
            tile->setPosition(lengthX, lengthY);
            if (tile->getType() != TileType::Empty)
                target.draw((sf::Sprite) *tile, states);
            lengthX += tile->getGlobalBounds().width;
            if (tile->getGlobalBounds().height != tileHeight) {
                if (isTileHeightChanged)
                    throw std::runtime_error("Tiles must be the same height!");
                tileHeight = tile->getGlobalBounds().height;
                isTileHeightChanged = true;
            }    
        }
        lengthY -= tileHeight;
    }
}
