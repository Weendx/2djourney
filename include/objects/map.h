#pragma once
#include <array>
#include <cstdint>
#include <vector>
#include <string>
#include "SFML/System/Vector2.hpp"
#include "object.h"
#include "objects/tile.h"

using TilesLengthArr = std::vector<std::array<float, 2>>;

struct LayerData {
    uint16_t tilesCount = 0;
    uint16_t id = 0;
    float startX = 0;
    float startY = 0;
    float endX = 0;
    float endY = 0;
    float tilesHeight = 0;
    TilesLengthArr tilesLength;  // [tileWidth, repeat]
};

class Map : public Object {
 public:
    Map();
    explicit Map(const int& bottom);
    virtual ~Map();
    void adjustScale(const sf::Vector2f &factors) override;
    void onUpdate(const sf::Time &deltaTime) override {}
    void handleEvent(const sf::Event &event) override {}
    operator std::string() const override { return "Map"; }
    void setBottom(const int& bottom) { m_bottom = bottom; }
    void createTiles();
    TileType getTileTypeAt(const sf::Vector2f& coords) const;
 private:
    std::vector<std::string> m_currentMapLayout;
    std::vector< std::vector<Tile*> > m_layers;
    std::vector<LayerData> m_layersData;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void fillMap();
    void deleteTiles();
    float getTileWidth(const TilesLengthArr& tilesLength, 
                                    const std::size_t& tileId) const;
    int m_bottom;
    sf::Vector2f m_scale {1.56, 1.56};
};
