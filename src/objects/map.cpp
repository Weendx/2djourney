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
    m_currentMapLayout.push_back("UYZYZYZYZYZYZYZYZYZX");
    m_currentMapLayout.push_back("T111111111111111111W");
    m_currentMapLayout.push_back("S111111111111111111V");
    m_currentMapLayout.push_back("T111111111111111111W");
    m_currentMapLayout.push_back("S111111111111111111V");
    m_currentMapLayout.push_back("T11111111PCBCK11111W");
    m_currentMapLayout.push_back("ABCBCBCBNO000LMBCBCD");
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
                auto last = layerData.tilesLength.back();
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
        }

        layerData.id = layerId;
        layerData.startX = startX;
        layerData.startY = startY;
        startY -= tileHeight;
        layerData.endX = layerLength;
        layerData.endY = startY;
        m_layersData.push_back(layerData);
        m_layers.push_back(layerTiles);
    }
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto layer : m_layers) {
        for (auto tile : layer) {
            target.draw((sf::Sprite) *tile, states);
        }
    }
}
