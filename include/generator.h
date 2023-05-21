#pragma once
#include <string>
#include <vector>
#include <map>

namespace sf {
template <typename T> class Vector2;
using Vector2f = Vector2<float>;
}

using GenResult = std::vector<std::vector<int>>;

class Generator {
 public:
    GenResult randomWalkTop(const int& width, const int& height, 
            const int& seed, const int& shiftX,
            const int& minSectionWidth, const int& maxSectionWidth);
    std::vector<std::string> to_string(const GenResult& gr);
    void test();

 private:
    std::map<int, int> m_height;  // seed: lastHeight

    template<typename T>
    float distance(sf::Vector2<T> p1, sf::Vector2<T> p2) const;
};
