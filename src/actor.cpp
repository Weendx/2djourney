#include "actor.h"
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_world.h>
#include <iostream>
#include <string>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "utils.h"


Actor::Actor(const sf::Texture& texture, 
                const sf::IntRect& rectangle, const bool &hasPhysics)
                                    : sf::Sprite(texture, rectangle) {
    m_hasPhysics = hasPhysics;
    init();
}
Actor::Actor(const sf::Texture& texture, const sf::IntRect& rectangle, 
                const sf::Vector2f& hitboxSizes, const bool &hasPhysics)
            : sf::Sprite(texture, rectangle), m_hitboxSizes(hitboxSizes) {
    m_hasPhysics = hasPhysics;
    init();
}

void Actor::init() {
    setOrigin(getGlobalBounds().getSize() / 2.0f);
}

void Actor::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw((sf::Sprite)*this, states);
}

void Actor::adjustScale(const sf::Vector2f &factors) {
    sf::Vector2f tmpScale(1.0, 1.0);
    tmpScale = this->getScale();
    tmpScale.x *= factors.x;
    tmpScale.y *= factors.y;
    this->setScale(tmpScale);
    m_hitboxSizes.x *= factors.x;
    m_hitboxSizes.y *= factors.y;
}

Actor::operator std::string() const {
    return "Actor [" + m_name + "]";
}

sf::Vector2f Actor::getHitBoxSize() {
    if (m_hitboxSizes.x != 0 && m_hitboxSizes.y != 0)
        return m_hitboxSizes;
    return this->getGlobalBounds().getSize();
}

void Actor::addPhysics(b2World* world) {
    b2BodyDef bdef;
    if (this->isItDynamic()) {
        bdef.type = b2_dynamicBody;
        bdef.fixedRotation = true;
    }

    bdef.position = coordPixelsToWorld(this->getPosition());
    b2Body* body = world->CreateBody(&bdef);

    b2PolygonShape ps;
    b2Vec2 hitboxsizes = coordPixelsToWorld(this->getHitBoxSize());
    ps.SetAsBox(hitboxsizes.x / 2.0f, hitboxsizes.y / 2.0f);


    b2FixtureDef fd;
    fd.shape = &ps;

    fd.density = 1;
    fd.friction = 1;
    fd.restitution = 0;

    body->CreateFixture(&fd);
    this->setBody(body);
}
