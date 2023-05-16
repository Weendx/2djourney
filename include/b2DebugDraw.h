#include "box2d/b2_draw.h"

namespace sf {
template <typename T> class Vector2;
using Vector2f = Vector2<float>;

class VertexArray;
class Color;
class RenderWindow;
}

class b2DebugDraw : public b2Draw {
 public:
    explicit b2DebugDraw(sf::RenderWindow* window);
    ~b2DebugDraw();
    void DrawPolygon(
            const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawSolidPolygon(
            const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawCircle(const b2Vec2& center, float radius, const b2Color& color);
    void DrawSolidCircle(const b2Vec2& center, 
            float radius, const b2Vec2& axis, const b2Color& color);
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
    void DrawTransform(const b2Transform& xf);
    void DrawPoint(const b2Vec2& p, float size, const b2Color& color);

 private:
    sf::RenderWindow* m_window;
};

sf::Color b2ColorToSFML(const b2Color& color, const int& alpha = 255);
