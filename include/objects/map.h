#pragma once
#include <array>
#include <cstdint>
#include <vector>
#include <string>
#include "SFML/System/Vector2.hpp"
#include "object.h"
#include "objects/tile.h"

class b2Body;
class b2Vec2;

struct LayerData {
    uint16_t tilesCount = 0;
    uint16_t id = 0;
    float startX = 0;
    float startY = 0;
    float endX = 0;
    float endY = 0;
    float tilesHeight = 0;
};

struct TileDef {
    TileDef() {
        width = 24;
        height = 24;
        density = 1;
        friction = 0.001;
        restitution = 0;
    }
    float width;
    float height;
    float density;
    float friction;
    float restitution;
};

class Map : public Object {
 public:
    Map();
    explicit Map(const int& bottom);
    virtual ~Map();
    void init();
    void adjustScale(const sf::Vector2f &factors) override;
    void onUpdate(const sf::Time &deltaTime) override {}
    void handleEvent(const sf::Event &event) override {}
    operator std::string() const override { return "Map"; }
    void setBottom(const int& bottom);
    void createTiles();
    bool checkGroundAt(const sf::Vector2f& coords) const;
    bool checkGroundAt(const b2Vec2& worldCoords) const;
    TileType getTileTypeAt(const sf::Vector2f& coords) const;

 private:
    std::vector<std::string> m_currentMapLayout;
    std::vector< std::vector<Tile*> > m_layers;
    std::vector<LayerData> m_layersData;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void fillMap();
    void deleteTiles();

    Tile* createTileAt(const float& tileId_x, 
                            const float& tileId_y, const TileType& type);
    Tile* updateTileAt(const float& tileId_x, 
                            const float& tileId_y, const TileType& newType);
    void addTileCollision(Tile* tile, 
                    const sf::Vector2f& tilePos, const sf::Vector2f& tileSize);

    sf::Vector2f getTileIdAt(const float& pixels_x, const float& pixels_y);
    void applyCollision();

    sf::Vector2f m_startPoint;
    sf::Vector2f m_scale {1.56, 1.56};
    TileDef defaultTileParams;

    b2Body* m_surface = nullptr;
    int m_surfaceFixturesCount = 0;
    // std::vector<Tile*> m_collisionTiles;
};
