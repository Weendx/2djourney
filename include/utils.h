#pragma once
#include <string>
#include "SFML/System/Vector2.hpp"
#include "box2d/b2_math.h"

// SFML stuff
namespace sf {
template <typename T> class Vector2;
using Vector2f = Vector2<float>;
}

// box2d stuff
class b2Vec2;


b2Vec2 coordPixelsToWorld(const sf::Vector2f& coords);
b2Vec2 coordPixelsToWorld(const float& x, const float& y);
sf::Vector2f coordWorldToPixels(const b2Vec2& coords);

b2Vec2 convVector2SFMLtoB2(const sf::Vector2f& sfmlVector2);
sf::Vector2f convVector2B2toSFML(const b2Vec2& box2dVector2);

float degToRad(const float& degrees);
float radToDeg(const float& radians);

std::string to_string_with_precision(const double a_value, const int n = 6);
