#include <iostream>
#include <stdexcept>
#include "SFML/Graphics/Rect.hpp"
#include "objects/map.h"
#include "objects/tile.h"

Map::Map() {
    fillMap();
}

Map::Map(const int& bottom) : m_bottom(bottom) {
    fillMap();
    createTiles();
}

Map::~Map() {
    for (auto layer : m_layers)
        for (auto tile : layer)
            delete tile;
}

void Map::adjustScale(const sf::Vector2f &factors) {
    deleteTiles();
    createTiles();
}

void Map::fillMap() {
    /*m_currentMapLayout.push_back("0");
    m_currentMapLayout.push_back("0");
    m_currentMapLayout.push_back("0UYZYZYZYZYZYZYZYZYZX");
    m_currentMapLayout.push_back("0T111111111111111111W");
    m_currentMapLayout.push_back("0S111111111111111111V");
    m_currentMapLayout.push_back("0T111111111111111111W");
    m_currentMapLayout.push_back("0S111111111111111111V");
    m_currentMapLayout.push_back("0T111111111111111111W");
    m_currentMapLayout.push_back("0ABCBCBCBCBCBCBCBCBCD");*/

    m_currentMapLayout.push_back("0");
    m_currentMapLayout.push_back("0");
    m_currentMapLayout.push_back("000000000000000000000");
    m_currentMapLayout.push_back("000000000000000000000");
    m_currentMapLayout.push_back("000000000000000000000");
    m_currentMapLayout.push_back("000000000000000000000");
    m_currentMapLayout.push_back("000000000000000000000");
    m_currentMapLayout.push_back("000000000000000000000");
    m_currentMapLayout.push_back("000000000000111100000");
    m_currentMapLayout.push_back("000000000000111100000");
    m_currentMapLayout.push_back("000000000000111100000");
    m_currentMapLayout.push_back("000000000000111100000");

}

void Map::deleteTiles() {
    for (auto layer : m_layers)
        for (auto tile : layer)
            delete tile;
    m_layersData.clear();
}

void Map::createTiles() {
    float startX = 0;
    float startY = m_bottom;
    float tileHeight = 0;
    bool isTileHeightChanged = false;
    uint16_t layerId = 0;

    // TODO(...): разбить на функции
    for (auto layer : m_currentMapLayout) {
        LayerData layerData;
        std::vector<Tile*> layerTiles;
        float layerLength = 0;
        
        startY -= tileHeight;
        
        for (char tileName : layer) {
            Tile* tile = new Tile( (TileType)tileName );
            tile->adjustScale(m_scale);
            sf::FloatRect tileBounds = tile->getGlobalBounds();

            if (tileBounds.height != tileHeight) {
                if (isTileHeightChanged)
                    throw std::runtime_error("Tiles must be the same height!");
                tileHeight = tileBounds.height;
                isTileHeightChanged = true;
            }

            tile->setPosition(startX + layerLength, startY);
            
            // Сохранение ширины тайлов
            if (!layerData.tilesLength.empty()) {
                std::array<float, 2>& last = layerData.tilesLength.back();
                if (last[0] == tileBounds.width) {
                    ++last[1];
                } else {
                    std::array<float, 2> arr {tileBounds.width, 1};
                    layerData.tilesLength.push_back(arr);
                }
            } else {
                std::array<float, 2> arr {tileBounds.width, 1};
                layerData.tilesLength.push_back(arr);
            }
            
            layerLength += tileBounds.width;
            ++layerData.tilesCount;
            layerTiles.push_back(tile);
            if(tile->getType() != TileType::Empty)
                m_collisionTiles.push_back(tile);
        }

        layerData.id = layerId;
        layerData.startX = startX;
        layerData.startY = startY;
        layerData.endX = layerLength;
        layerData.endY = startY + tileHeight;
        layerData.tilesHeight = tileHeight;
        m_layersData.push_back(layerData);
        m_layers.push_back(layerTiles);
    }
}

float Map::getTileWidth(const TilesLengthArr& tilesLength, 
                                    const std::size_t& tileId) const {
    for (auto part : tilesLength) {
        if ( (tileId + 1) - part[1] <= 0 ) {
            return part[0];  // //
        }
    }
    throw std::logic_error("Can't find the tile width");
}

TileType Map::getTileTypeAt(const sf::Vector2f& coords) const {
    for (int i = m_layers.size() - 1; i >= 0; --i) {
        if (!(coords.y >= m_layersData[i].startY && 
                        coords.y <= m_layersData[i].endY)) {
            continue;
        }

        float marginX = 0;
        for (std::size_t j = 0; j < m_layersData[i].tilesCount; ++j) {
            float width = getTileWidth(m_layersData[i].tilesLength, j);
            if (coords.x >= marginX && coords.x <= (marginX + width)) {
                return m_layers[i][j]->getType();
            }
            marginX += width;
        }
    }
    return TileType::Empty;
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::Vector2f hu(75, 496);
    for (auto layer : m_layers) {
        for (auto tile : layer) {
            target.draw((sf::Sprite) *tile, states);
        }
    }
}
