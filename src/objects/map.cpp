#include "objects/map.h"

#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_chain_shape.h>
#include <box2d/b2_fixture.h>

#include <cstdint>
#include <iostream>
#include <list>
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

Map::Map(Core* core) {
    setCoreInstance(core);
    fillMap();
}

Map::~Map() {
    deleteMap();
    if (tilePointer)
        delete tilePointer;
}

void Map::init() {
    if (!m_surface) {
        createSurface();
    }
    if (!tilePointer) 
        tilePointer = new sf::RectangleShape(sf::Vector2f(24, 24));
    tilePointer->setFillColor(sf::Color::Transparent);
    createTiles();
}

void Map::adjustScale(const sf::Vector2f &factors) {
    deleteMap();
    createTiles();
}

void Map::onUpdate(const sf::Time &deltaTime) {
    for (auto cd : m_coins) {
        m_layers[cd.pos.y][cd.pos.x]->onUpdate(deltaTime);
    }
}

void Map::fillMap() {
    m_defaultMapLayout.push_back("0");
    m_defaultMapLayout.push_back("0");
    m_defaultMapLayout.push_back("0UYZYZYZYZYZYZYZYZYZYZYZYZYZYZYZYZYZYZYZYZX");
    m_defaultMapLayout.push_back("0T1111111111111111111111111111111111111111W");
    m_defaultMapLayout.push_back("0S1111111111111111111111111111111111111111V");
    m_defaultMapLayout.push_back("0T1111111111111111111111111111111111111111W");
    m_defaultMapLayout.push_back("0S1111111111111111111111111111111111111111V");
    m_defaultMapLayout.push_back("0T1111111111111111111111111111111111111111W");
    m_defaultMapLayout.push_back("0ABCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBC11111111r");
    m_defaultMapLayout.push_back("000000000000000000000kk00kkk000000CBC11111r");
    m_defaultMapLayout.push_back("0000000000000000000000000000000000000BC111r");
    m_defaultMapLayout.push_back("000000000000000000000000000000000000000BCBr");
    m_defaultMapLayout.push_back("0000000000FIJIJH00000000000000000000000000r");
    for (int i = 0; i < 3; ++i)
        m_defaultMapLayout.push_back("0");
    for (int i = 0; i < 10; ++i)
        m_defaultMapLayout.push_back("00000000000000000000e");
}

void Map::createSurface() {
    b2BodyDef bd;
    m_surface = m_coreInstance->getWorld()->CreateBody(&bd);
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
    Tile* tile = m_layers[tileId_y][tileId_x];
    // if (tile->hasCollision()) {
    //     m_coreInstance->getWorld()->DestroyBody(tile->getBody());
    // }
    delete tile;
    tile = createTileAt(tileId_x, tileId_y, newType);
    m_layers[tileId_y][tileId_x] = tile;
    return tile;
}

void Map::applyCollision(const sf::Vector2f& start, const sf::Vector2f& end) {
    // Функция должна вызываться после добавления новых тайлов в m_layers
    // Схема работы: с нижнего слоя с первого элемента и до конца
    if (!m_surface)
        throw std::logic_error("Map::m_surface is nullptr.");
    b2Vec2 vertices[6];
    int vertCount = 0;
    TileClass lastTileClass = Void;
    float blockWidth = 7;  // real width is blockWidth + world_tile_width
    for (int y = start.y; y <= end.y; ++y) {
        for (int x = start.x; x <= end.x; ++x) {
            auto tileBounds = m_layers[y][x]->getGlobalBounds();
            auto tileClass = m_layers[y][x]->getClass();
            if (vertCount > 3 && (tileClass != lastTileClass \
                        || vertices[2].x - vertices[1].x >= blockWidth)) {
                int fixtureId = 99;
                bool isSensor = false;
                switch (lastTileClass) {
                    case Spike:
                        fixtureId = 12;
                        isSensor = true;
                        break;
                    case Reward:
                        fixtureId = 13;
                        isSensor = true;
                        break;
                    default:
                        break;
                }
                auto fi = createBlock(vertices, vertCount, fixtureId, isSensor);
                if (lastTileClass == Reward) {
                    m_coins.push_back({fi, {x-1, y}});  // -1 for prev iter
                    if (x-1 >= end.x)
                        std::cout << "here x is " << x << std::endl;
                }
                vertCount = 0;
            }
            if (tileClass != Void) {
                if (vertCount == 0) {
                    vertices[0] = coordPixelsToWorld(
                        tileBounds.left, tileBounds.top + tileBounds.height);
                    vertices[1] = coordPixelsToWorld(
                                            tileBounds.left, tileBounds.top);
                    vertCount = 2;
                }
                vertices[2] = coordPixelsToWorld(
                        tileBounds.left + tileBounds.width, tileBounds.top);
                vertices[3] = coordPixelsToWorld(
                                tileBounds.left + tileBounds.width, 
                                        tileBounds.top + tileBounds.height);
                vertCount = 4;
            }
            lastTileClass = tileClass;
        }
        if (vertCount == 4) {
            int fixtureId = 99;
            bool isSensor = false;
            switch (lastTileClass) {
                case Spike:
                    fixtureId = 12;
                    isSensor = true;
                    break;
                case Reward:
                    fixtureId = 13;
                    isSensor = true;
                    break;
                default:
                    break;
            }
            auto fi = createBlock(vertices, vertCount, fixtureId, isSensor);
            if (lastTileClass == Reward)
                m_coins.push_back({fi, {static_cast<int>(end.x), y}});
        }
        vertCount = 0;
    }
    
    // Create checkpoint for map loading
    auto tB = m_layers[end.y][end.x]->getGlobalBounds();
    int s = end.y;
    vertices[0] = coordPixelsToWorld(tB.left, tB.top + tB.height * s);
    vertices[1] = coordPixelsToWorld(tB.left, tB.top - tB.height * s);
    vertices[2] = coordPixelsToWorld(tB.left + tB.width, tB.top - tB.height*s);
    vertices[3] = coordPixelsToWorld(tB.left + tB.width, tB.top + tB.height*s);
    b2Fixture* checkpoint = createBlock(vertices, 4, 98, true);
    m_checkpoints.push_back(checkpoint);
}

b2Fixture* Map::createBlock(const b2Vec2 *vertices, 
        int vertCount, int fixtureId, bool isSensor) {
    b2ChainShape shape;
    shape.CreateLoop(vertices, vertCount);

    b2FixtureDef fixdef;
    fixdef.shape = &shape;
    fixdef.density = defaultTileParams.density;
    fixdef.friction = defaultTileParams.friction;
    fixdef.restitution = defaultTileParams.restitution;
    fixdef.isSensor = isSensor;
    b2FixtureUserData udata;
    udata.pointer = (uintptr_t) fixtureId;
    fixdef.userData = udata;

    return m_surface->CreateFixture(&fixdef);
}

sf::Vector2f Map::shiftTileId(const sf::Vector2f& tileId) const {
    sf::Vector2f newTID(tileId);
    newTID.x -= m_lastCreatedTileId.x;
    newTID.y -= m_lastCreatedTileId.y;
    return newTID;
}

void Map::createTiles() {
    if (!m_surface)
        createSurface();
    sf::Vector2f lastTileId;
    sf::Vector2f tileSize;
    tileSize.x = defaultTileParams.width * m_scale.x;
    tileSize.y = defaultTileParams.width * m_scale.y;

    if (tilePointer)
        tilePointer->setSize(tileSize);

    for (auto layer : m_defaultMapLayout) {
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

    sf::Vector2f start(0, 0);
    sf::Vector2f end(m_layers.back().size()-1, m_layers.size()-1);
    applyCollision(start, end);

    // А вот не выйдешь ты больше слева
    b2Vec2 vert[4];
    int space = 20;
    vert[0] = coordPixelsToWorld(m_startPoint.x - 24, m_startPoint.y);
    vert[0].y += space;
    vert[1] = coordPixelsToWorld(m_startPoint.x - 24, m_startPoint.y);
    vert[1].y -= space;
    vert[2] = coordPixelsToWorld(m_startPoint.x, m_startPoint.y);
    vert[2].y -= space;
    vert[3] = coordPixelsToWorld(m_startPoint.x, m_startPoint.y);
    vert[3].y += space;
    createBlock(vert, 4, 99);
    // m_layers[6][2] = updateTileAt(2, 6, TileType::GrassSlopeDown3);
}

void Map::deleteMap() {
    for (auto layer : m_layers)
        for (auto tile : layer)
            delete tile;
    m_layers.clear();
    m_layersData.clear();
    m_coreInstance->getWorld()->DestroyBody(m_surface);
    m_surface = nullptr;
    m_checkpoints.clear();
    m_coins.clear();
    m_lastCreatedTileId = {0, 0};
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
    m_defaultMapLayout = layout;
}

void Map::appendMapLayout(const std::vector<std::string> &layout) {
    // Предположим, что на вход нам всегда приходит 
    // прямоугольная фигура, соразмерная со старыми размерами
    if (layout.size() == 0)
        return;

    sf::Vector2f startTileId, endTileId;
    sf::Vector2f tileSize;
    tileSize.x = defaultTileParams.width * m_scale.x;
    tileSize.y = defaultTileParams.width * m_scale.y;
    startTileId.x = m_layersData.back().tilesCount;

    for (int x = 0; x < layout.back().size(); ++x) {
        for (int y = 0; y < layout.size(); ++y) {
            m_layers[y].push_back(createTileAt(
                        startTileId.x + x, y, (TileType) layout[y][x]));
            m_layersData[y].endX += tileSize.x;
            ++m_layersData[y].tilesCount;
            m_lastCreatedTileId.x = startTileId.x + x;
            m_lastCreatedTileId.y = y;
        }
    } 

    startTileId.y = 0;
    endTileId.x = startTileId.x + layout.back().size() - 1;
    endTileId.y = layout.size() - 1;
    applyCollision(startTileId, endTileId);
}

sf::Vector2f Map::getStartPoint() {
    return getStartPoint(m_layers.back().size() / 2 - 10);
}

sf::Vector2f Map::getStartPoint(const int& atX) {
    int lastLayerId = m_layers.size() - 1;
    if (lastLayerId == -1)
        throw std::logic_error("Map::getUpperPoint(): Tiles vector is empty");
    sf::Vector2f tilePos;
    TileType types[4];  // над
    for (int x = atX+1; x < m_layers.back().size()-2; ++x) {
        for (int y = m_layers.size() - 2; y >= 0; --y) {
            // if (m_layers[y][x]->getType() == Empty)
                // continue;
            types[0] = m_layers[y][x]->getType();
            types[1] = m_layers[y+1][x-1]->getType();
            types[2] = m_layers[y+1][x]->getType();
            types[3] = m_layers[y+1][x+1]->getType();
            if (types[0] == Empty)
                continue;
            if (types[0] == Coin || types[0] == Spikes)
                break;
            // if ((types[1] != Empty || types[2] != Empty || types[3] != Empty)
            if ((types[1] == Spikes || types[2] == Spikes || types[3] == Spikes))
                break;  // Danger zone
            
            tilePos = m_layers[y][x]->getPosition();
            this->updateTileAt(x, y, (TileType) 'G');
            return sf::Vector2f(tilePos.x + 12, tilePos.y - 24.f);
        }
    }
    return sf::Vector2f();
    return sf::Vector2f(tilePos.x + 14.f, tilePos.y - 24);
}

void Map::putStartPoint(const sf::Vector2f &coords) {
    sf::Vector2f tId;
    tId.x = coords.x / (defaultTileParams.width * m_scale.x);
    tId.y = coords.y / (defaultTileParams.height * m_scale.y);
    this->updateTileAt(tId.x, tId.y, TileType::GrassSingle);
}

bool Map::removeCheckpoint(b2Fixture* checkpoint) {
    std::list<b2Fixture*>::iterator it = m_checkpoints.begin();
    for (auto f : m_checkpoints) {
        if (f == checkpoint) {
            m_surface->DestroyFixture(f);
            f = nullptr;
            m_checkpoints.erase(it);
            return true;
        }
        ++it;
    }
    return false;
}

bool Map::removeCoin(b2Fixture* coin) {
    std::list<CoinData>::iterator it = m_coins.begin();
    for (auto cd : m_coins) {
        if (cd.fixture == coin) {
            m_surface->DestroyFixture(cd.fixture);
            cd.fixture = nullptr;
            m_layers[cd.pos.y][cd.pos.x] = updateTileAt(
                            cd.pos.x, cd.pos.y, TileType::Empty);
            m_coins.erase(it);
            return true;
        }
        ++it;
    }
    return false;
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto layer : m_layers) {
        for (auto tile : layer) {
            if (tile->getType() != Empty)
                target.draw((sf::Sprite) *tile, states);
        }
    }
    if (showDebug && tilePointer)
        target.draw(*tilePointer, states);
}

void Map::setBottom(const int &bottom) {
    m_startPoint.y = bottom;
}
