#pragma once
#include <cstdint>
#include <vector>
#include <list>
#include <string>
#include "SFML/System/Vector2.hpp"
#include "object.h"
#include "objects/tile.h"

class b2Body;
class b2Vec2;
class b2Fixture;

struct LayerData {
    uint16_t tilesCount = 0;
    uint16_t id = 0;
    float startX = 0;
    float startY = 0;
    float endX = 0;
    float endY = 0;
    float tilesHeight = 0;
};

struct CoinData {
    b2Fixture* fixture;
    sf::Vector2i pos;
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

class Map : virtual public Object {
 public:
    explicit Map(Core* core);
    virtual ~Map();
    void init();
    void adjustScale(const sf::Vector2f &factors) override;
    void onUpdate(const sf::Time &deltaTime) override;
    void handleEvent(const sf::Event &event) override {}
    operator std::string() const override { return "Map"; }
    void setBottom(const int& bottom);
    void createTiles();
    void deleteMap();
    bool checkGroundAt(const sf::Vector2f& coords) const;
    bool checkGroundAt(const b2Vec2& worldCoords) const;
    TileType getTileTypeAt(const sf::Vector2f& coords) const;

    void setMapLayout(const std::vector<std::string>& layout);
    void appendMapLayout(const std::vector<std::string>& layout);
    sf::Vector2f getStartPoint();
    sf::Vector2f getStartPoint(const int& atX);
    void putStartPoint(const sf::Vector2f& coords);

    bool removeCheckpoint(b2Fixture* checkpoint);
    bool removeCoin(b2Fixture* coin);

 private:
    std::vector<std::string> m_defaultMapLayout;
    std::vector< std::vector<Tile*> > m_layers;
    std::vector<LayerData> m_layersData;
    std::list<b2Fixture*> m_checkpoints;
    std::list<CoinData> m_coins;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void fillMap();

    void createSurface();

    Tile* createTileAt(const float& tileId_x, 
                            const float& tileId_y, const TileType& type);
    Tile* updateTileAt(const float& tileId_x, 
                            const float& tileId_y, const TileType& newType);
    void addTileCollision(Tile* tile, 
                    const sf::Vector2f& tilePos, const sf::Vector2f& tileSize);

    sf::Vector2f getTileIdAt(const float& pixels_x, const float& pixels_y);
    void applyCollision();
    void applyCollision(const sf::Vector2f& start, const sf::Vector2f& end);
    b2Fixture* createBlock(const b2Vec2 *vertices, 
                            int count, int fixtureId, bool isSensor = false);

    sf::Vector2f shiftTileId(const sf::Vector2f& tileIds) const;

    sf::Vector2f m_startPoint;
    sf::Vector2f m_scale {1.56, 1.56};
    TileDef defaultTileParams;

    b2Body* m_surface = nullptr;
    int m_surfaceFixturesCount = 0;

    sf::Vector2f m_lastCreatedTileId;

    // std::vector<Tile*> m_collisionTiles;
};
