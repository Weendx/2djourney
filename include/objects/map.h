#include <vector>
#include <string>
#include "object.h"
#include "objects/tile.h"

class Map : public Object {
 public:
    Map();
    Map(const int& bottom);
    virtual ~Map();
    void adjustScale(const sf::Vector2f &factors) override;
    void onUpdate(const sf::Time &deltaTime) override {}
    void handleEvent(const sf::Event &event) override {}
    operator std::string() const override { return "Map"; }
    void setBottom(const int& bottom) { m_bottom = bottom; }
 private:
    std::vector<std::string> m_currentMapLayout;
    std::vector< std::vector<Tile*> > m_layers;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void fillMap();
    void createTiles();
    int m_bottom;
    sf::Vector2f m_scale {1.2, 1.2};
};
