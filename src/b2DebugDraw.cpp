#include "SFML/Config.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/ConvexShape.hpp"
#include "SFML/Graphics/PrimitiveType.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Vertex.hpp"
#include "SFML/Graphics/VertexArray.hpp"

#include "SFML/System/Vector2.hpp"
#include "b2DebugDraw.h"
#include "utils.h"

b2DebugDraw::b2DebugDraw(sf::RenderWindow* window) : m_window(window) {}

b2DebugDraw::~b2DebugDraw() {}

void b2DebugDraw::DrawPolygon(
        const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    sf::ConvexShape convShape;
    convShape.setPointCount(vertexCount);
    for (int32 i = 0; i < vertexCount; ++i) {
        convShape.setPoint(i, coordWorldToPixels(vertices[i]));
    }
    convShape.setFillColor(sf::Color::Transparent);
    convShape.setOutlineThickness(1.0);
    convShape.setOutlineColor(b2ColorToSFML(color));
    m_window->draw(convShape);
}

void b2DebugDraw::DrawSolidPolygon(
        const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    sf::ConvexShape convShape;
    convShape.setPointCount(vertexCount);
    for (int32 i = 0; i < vertexCount; ++i) {
        convShape.setPoint(i, coordWorldToPixels(vertices[i]));
    }
    sf::Color outlineColor = b2ColorToSFML(color);
    sf::Color fillColor = b2ColorToSFML(color, 20);
    convShape.setFillColor(b2ColorToSFML(color, 20));
    convShape.setOutlineThickness(1.0);
    convShape.setOutlineColor(b2ColorToSFML(color));
    m_window->draw(convShape);
}

void b2DebugDraw::DrawCircle(const b2Vec2& center, 
                    float radius, const b2Color& color) {
    sf::CircleShape circle(radius);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(1.0);
    circle.setOutlineColor(b2ColorToSFML(color));
    circle.setPosition(coordWorldToPixels(center));
    m_window->draw(circle);
}

void b2DebugDraw::DrawSolidCircle(const b2Vec2& center, 
        float radius, const b2Vec2& axis, const b2Color& color) {
    sf::CircleShape circle(radius);
    circle.setFillColor(b2ColorToSFML(color, 20));
    circle.setOutlineThickness(1.0);
    circle.setOutlineColor(b2ColorToSFML(color));
    circle.setPosition(coordWorldToPixels(center));
    m_window->draw(circle);

    // Draw a line fixed in the circle to animate rotation.
    b2Vec2 p = center + radius * axis;
    DrawSegment(center, p, color);
}

void b2DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, 
                                                    const b2Color& color) {
    sf::VertexArray vArr(sf::LineStrip);
    vArr.append(sf::Vertex(coordWorldToPixels(p1), b2ColorToSFML(color)));
    vArr.append(sf::Vertex(coordWorldToPixels(p2), b2ColorToSFML(color)));
    m_window->draw(vArr);
}

void b2DebugDraw::DrawTransform(const b2Transform& xf) {
    // example: Center of Mass
    sf::VertexArray xAxis(sf::LineStrip), yAxis(sf::LineStrip);
    
    b2Vec2 secondPoint;
    const float axisScale = 0.4;
    sf::Vector2f origin = coordWorldToPixels(xf.p);
    secondPoint = xf.p + axisScale * xf.q.GetXAxis();
    xAxis.append(sf::Vertex(origin, sf::Color::Red));
    xAxis.append(sf::Vertex(coordWorldToPixels(secondPoint), sf::Color::Red));
    secondPoint = xf.p - axisScale * xf.q.GetYAxis();
    yAxis.append(sf::Vertex(origin, sf::Color::Green));
    yAxis.append(sf::Vertex(coordWorldToPixels(secondPoint), sf::Color::Green));
    m_window->draw(xAxis);
    m_window->draw(yAxis);
}

void b2DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color) {
    sf::Vector2f sizes(size / 2.0, size / 2.0);
    sf::RectangleShape point(sizes);
    point.setOrigin(sizes);
    point.setOutlineThickness(0.0);
    point.setFillColor(b2ColorToSFML(color));
    point.setPosition(coordWorldToPixels(p));
    m_window->draw(point);
}

sf::Color b2ColorToSFML(const b2Color &color, const int& alpha) {
    return sf::Color((sf::Uint8)(color.r * 255), (sf::Uint8)(color.g * 255), 
                    (sf::Uint8)(color.b * 255), (sf::Uint8)(color.a * alpha));
}
