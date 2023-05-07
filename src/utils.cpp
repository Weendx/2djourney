#include "utils.h"
#include "SFML/System/Vector2.hpp"
#include "box2d/b2_math.h"

namespace {
    float factor = 24.0;
}

b2Vec2 coordPixelsToWorld(const sf::Vector2f& coords) {
    b2Vec2 b2vec2;
    b2vec2.Set(coords.x / factor, coords.y / factor);
    return b2vec2;
}
b2Vec2 coordPixelsToWorld(const float& x, const float& y) {
    b2Vec2 b2vec2;
    b2vec2.Set(x / factor, y / factor);
    return b2vec2;
}
sf::Vector2f coordWorldToPixels(const b2Vec2& coords) {
    return sf::Vector2f({coords.x * factor, coords.y * factor});
}

b2Vec2 convVector2SFMLtoB2(const sf::Vector2f& sfmlVector2) {
    return b2Vec2(sfmlVector2.x, sfmlVector2.y);
}
sf::Vector2f convVector2B2toSFML(const b2Vec2& box2dVector2) {
    return sf::Vector2f(box2dVector2.x, box2dVector2.y);
}
