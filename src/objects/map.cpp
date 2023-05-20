#include "objects/map.h"

#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_chain_shape.h>
#include <box2d/b2_fixture.h>

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "objects/tile.h"
#include "core.h"
#include "utils.h"


sf::RectangleShape* tilePointer = nullptr;

Map::Map() {
    fillMap();
}

// potential BUG place here

Map::Map(const int& bottom) : m_startPoint(0, bottom) {
    fillMap();
    init();    
}

Map::~Map() {
    deleteTiles();
    if (tilePointer)
        delete tilePointer;
}

void Map::init() {
    if (!m_surface) {
        b2BodyDef bd;
        // bd.position = coordPixelsToWorld(m_startPoint);
        m_surface = m_coreInstance->getWorld()->CreateBody(&bd);
    }
    
    tilePointer = new sf::RectangleShape(sf::Vector2f(24, 24));
    tilePointer->setFillColor(sf::Color::Transparent);
    createTiles();
}

void Map::adjustScale(const sf::Vector2f &factors) {
    deleteTiles();
    createTiles();
}

void Map::fillMap() {
    m_currentMapLayout.push_back("0");
    m_currentMapLayout.push_back("0");
    m_currentMapLayout.push_back("0UYZYZYZYZYZYZYZYZYZYZYZYZYZYZYZYZYZYZYZYZX");
    m_currentMapLayout.push_back("0T1111111111111111111111111111111111111111W");
    m_currentMapLayout.push_back("0S1111111111111111111111111111111111111111V");
    m_currentMapLayout.push_back("0T1111111111111111111111111111111111111111W");
    m_currentMapLayout.push_back("0S1111111111111111111111111111111111111111V");
    m_currentMapLayout.push_back("0T1111111111111111111111111111111111111111W");
    m_currentMapLayout.push_back("0ABCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBC11111111r");
    m_currentMapLayout.push_back("000000000000000000000kk00kkk000000CBC11111r");
    m_currentMapLayout.push_back("0000000000000000000000000000000000000BC111r");
    m_currentMapLayout.push_back("000000000000000000000000000000000000000BCBr");
    m_currentMapLayout.push_back("0000000000FIJIJH00000000000000000000000000r");
    for (int i = 0; i < 3; ++i)
        m_currentMapLayout.push_back("0");
    for (int i = 0; i < 10; ++i)
        m_currentMapLayout.push_back("00000000000000000000e");
}

void Map::deleteTiles() {
    for (auto layer : m_layers)
        for (auto tile : layer)
            delete tile;
    m_layersData.clear();
}

sf::Vector2f getTileIdAt(const float& x, const float& y) {
    return sf::Vector2f();
}

Tile* Map::createTileAt(const float& tileId_x, 
                        const float& tileId_y, const TileType& type) {
    Tile* tile = new Tile(type);
    sf::Vector2f tileLocalSize = tile->getLocalBounds().getSize();
    sf::Vector2f tileSize(defaultTileParams.width, defaultTileParams.height);
    if (tileLocalSize != tileSize) {
        std::string sizes = '(' + std::to_string(tileLocalSize.x) + ',' + ' ' \
            + std::to_string(tileLocalSize.y) + ')';
        throw std::logic_error("Invalid tile size in Map. Got " + sizes);
    }
    sf::Vector2f tilePos;
    tileSize.x *= m_scale.x;
    tileSize.y *= m_scale.y;
    tilePos.x = m_startPoint.x + tileId_x * tileSize.x;
    tilePos.y = m_startPoint.y - (tileId_y + 1) * tileSize.y;
    
    tile->setPosition(tilePos);
    tile->adjustScale(m_scale);
    // addTileCollision(tile, tilePos, tileSize);
    return tile;
}

void Map::addTileCollision(Tile* tile, 
                const sf::Vector2f& tilePos, const sf::Vector2f& tileSize) {
    if (!m_coreInstance)
        throw std::logic_error("Map object doesn't know a Core instance.");
    if (!tile->hasCollision())
        return;

    b2Vec2 b2TilePos = coordPixelsToWorld(tilePos);
    b2Vec2 b2TileSize = coordPixelsToWorld(tileSize);

    // Local center of the tile
    b2Vec2 center(b2TileSize.x / 2.0, b2TileSize.y / 2.0);

    b2BodyDef bdef;
    bdef.position.Set(b2TilePos.x + center.x, b2TilePos.y + center.y);
    b2Body* body = m_coreInstance->getWorld()->CreateBody(&bdef);
    tile->setBody(body);

    b2PolygonShape pshape;
    pshape.SetAsBox(center.x, center.y, b2Vec2(0, 0), 0);

    b2FixtureDef fixdef;
    fixdef.shape = &pshape;
    fixdef.density = defaultTileParams.density;
    fixdef.friction = defaultTileParams.friction;
    fixdef.restitution = defaultTileParams.restitution;
    
    body->CreateFixture(&fixdef);
    // m_surface->CreateFixture(&pshape, 0);
}

Tile* Map::updateTileAt(const float& tileId_x, 
                        const float& tileId_y, const TileType& newType) {
    if (newType == TileType::Empty)
        throw std::domain_error("This action isn't allowed!");
    Tile* tile = m_layers[tileId_y][tileId_x];
    // if (tile->hasCollision()) {
    //     m_coreInstance->getWorld()->DestroyBody(tile->getBody());
    // }
    delete tile;
    tile = createTileAt(tileId_x, tileId_y, newType);
    return tile;
}

void Map::applyCollision() {
    if (!m_surface)
        throw std::logic_error("Map::m_surface is nullptr.");
    for (auto& layer : m_layers) {
        const int MAX_VERTICES_AT_ONE_SHAPE = 30;
        b2Vec2 vertices[MAX_VERTICES_AT_ONE_SHAPE];
        int vertCount = 0;
        float shapeLength = 0.0;
        sf::Vector2f lastTileRightCoord;
        sf::Vector2f lastTileBottomCoord;
        TileType lastTileType;
        for (auto& tile : layer) {
            if (tile->getType() == TileType::Spikes && vertCount) {
                vertices[vertCount++] = coordPixelsToWorld(lastTileRightCoord);
                vertices[vertCount++] = coordPixelsToWorld(lastTileBottomCoord);
                int fixtureId = 99;
                if (lastTileType == TileType::Spikes)
                    fixtureId = 12;
                createColBlock(vertices, vertCount, fixtureId); 

                ++m_surfaceFixturesCount;
                shapeLength = 0;
                vertCount = 0;
            }
            lastTileType = tile->getType();
            if (tile->hasCollision()) {
                sf::Vector2f tilePos = tile->getPosition();
                sf::Vector2f tileSize = tile->getGlobalBounds().getSize();
                
                if (!vertCount) {
                    vertices[vertCount++] = coordPixelsToWorld(
                        tilePos.x, tilePos.y + tileSize.y);
                    vertices[vertCount++] = coordPixelsToWorld(tilePos);
                }

                lastTileRightCoord.x = tilePos.x + tileSize.x;
                lastTileRightCoord.y = tilePos.y;
                lastTileBottomCoord.x = tilePos.x + tileSize.x;
                lastTileBottomCoord.y = tilePos.y + tileSize.y;
                
                b2Vec2 b2TileSize = coordPixelsToWorld(tileSize);
                shapeLength += b2TileSize.x;  // ugrh
            }
            if (!tile->hasCollision() || shapeLength > 7 || \
                        vertCount + 3 > MAX_VERTICES_AT_ONE_SHAPE || \
                        lastTileType == TileType::Spikes) {
                if (!vertCount)
                    continue;
                vertices[vertCount++] = coordPixelsToWorld(lastTileRightCoord);
                vertices[vertCount++] = coordPixelsToWorld(lastTileBottomCoord);
                int fixtureId = 99;
                if (lastTileType == TileType::Spikes)
                    fixtureId = 12;
                createColBlock(vertices, vertCount, fixtureId); 

                ++m_surfaceFixturesCount;
                shapeLength = 0;
                vertCount = 0;
            } 
        }
        // Copy-paste is evil!!
        if (!vertCount)
            continue;
        vertices[vertCount++] = coordPixelsToWorld(lastTileRightCoord);
        vertices[vertCount++] = coordPixelsToWorld(lastTileBottomCoord);
        
        vertices[vertCount++] = coordPixelsToWorld(lastTileRightCoord);
        vertices[vertCount++] = coordPixelsToWorld(lastTileBottomCoord);
        int fixtureId = 99;
        if (lastTileType == TileType::Spikes)
            fixtureId = 12;
        createColBlock(vertices, vertCount, fixtureId);
        ++m_surfaceFixturesCount;
        shapeLength = 0;
        vertCount = 0;
    }
}

void Map::createColBlock(const b2Vec2 *vertices, int vertCount, int fixtureId) {
    if (!vertCount)
        return;
    
    b2ChainShape shape;
    shape.CreateLoop(vertices, vertCount);

    b2FixtureDef fixdef;
    fixdef.shape = &shape;
    fixdef.density = defaultTileParams.density;
    fixdef.friction = defaultTileParams.friction;
    fixdef.restitution = defaultTileParams.restitution;
    b2FixtureUserData udata;
    udata.pointer = (uintptr_t) fixtureId;
    fixdef.userData = udata;

    m_surface->CreateFixture(&fixdef);
}

void Map::createTiles() {
    sf::Vector2f lastTileId;
    sf::Vector2f tileSize;
    tileSize.x = defaultTileParams.width * m_scale.x;
    tileSize.y = defaultTileParams.width * m_scale.y;

    if (tilePointer)
        tilePointer->setSize(tileSize);

    for (auto layer : m_currentMapLayout) {
        std::vector<Tile*> layerTiles;
        LayerData layerData;
        for (char tileType : layer) {
            Tile* tile = createTileAt(
                    lastTileId.x, lastTileId.y, (TileType) tileType);
            layerTiles.push_back(tile);
            ++lastTileId.x;
        }
        
        layerData.id = lastTileId.y;
        layerData.tilesCount = lastTileId.x;
        layerData.startX = m_startPoint.x;
        layerData.startY = m_startPoint.y - tileSize.y * (lastTileId.y + 1);
        layerData.endX = m_startPoint.x + tileSize.x * lastTileId.x;
        layerData.endY = layerData.startY + tileSize.y;

        m_layers.push_back(layerTiles);
        m_layersData.push_back(layerData);
        lastTileId.x = 0;
        ++lastTileId.y;
    }

    applyCollision();
    // m_layers[6][2] = updateTileAt(2, 6, TileType::GrassSlopeDown3);
}

TileType Map::getTileTypeAt(const sf::Vector2f& coords) const {
    for (int i = m_layers.size() - 1; i >= 0; --i) {
        if (tilePointer)
            tilePointer->setFillColor(sf::Color::Transparent);

        if (!(coords.y >= m_layersData[i].startY && 
                        coords.y <= m_layersData[i].endY)) {
            continue;
        }
        float marginX = m_layersData[i].startX;
        for (std::size_t j = 0; j < m_layersData[i].tilesCount; ++j) {
            float width = defaultTileParams.width * m_scale.x;
            if (tilePointer) {
                tilePointer->setPosition(marginX, m_layersData[i].startY);
                tilePointer->setFillColor(sf::Color(0, 255, 0, 100));
            }
            if (coords.x >= marginX && coords.x <= (marginX + width)) {
                return m_layers[i][j]->getType();
            }
            marginX += width;
        }
    }
    return TileType::Empty;
}

void Map::setMapLayout(const std::vector<std::string> &layout) {
    m_currentMapLayout = layout;
}

sf::Vector2f Map::getUpperPoint() const {
    int lastLayerId = m_layers.size() - 1;
    if (lastLayerId == -1)
        throw std::logic_error("Map::getUpperPoint(): Tiles vector is empty");
    sf::Vector2f tilePos;
    for (int y = m_layers.size() - 1; y >= 0; --y) {
        if (m_layers[y][0]->getType() == Empty)
            continue;
        for (int x = 0; x < m_layers[lastLayerId].size(); ++x) {
            if (m_layers[y][x]->getType() == Spikes)
                continue;
            tilePos = m_layers[y][x]->getPosition();
            return sf::Vector2f(tilePos.x + 14.f, tilePos.y);
        }
    }
    return sf::Vector2f(tilePos.x + 14.f, tilePos.y);
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto layer : m_layers) {
        for (auto tile : layer) {
            target.draw((sf::Sprite) *tile, states);
        }
    }
    if (showDebug && tilePointer)
        target.draw(*tilePointer, states);
}

void Map::setBottom(const int &bottom) {
    m_startPoint.y = bottom;
}
