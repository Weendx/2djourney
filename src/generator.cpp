#include "generator.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <SFML/System/Vector2.hpp>

GenResult Generator::randomWalkTop(const int& width, const int& height, 
                        const int& seed, const int& shiftX, 
                            const int& minSecWidth, const int& maxSecWidth) {
    std::srand(seed);
    GenResult map;
    int lastHeight = std::rand() % height;
    lastHeight = m_height.count(seed) ? m_height[seed] : lastHeight;
    int prevHeight = lastHeight;
    int sectionWidth = (std::rand() % (maxSecWidth - minSecWidth));
    sectionWidth += minSecWidth;
    int currSecWidth = 0;
    bool hasSpike = false;
    bool isStart = false;
    bool isEnd = false;
    int spikesLength = 0;
    sf::Vector2i lastSpikeEnd;
    sf::Vector2i lastCoin;

    for (int i = 0; i < shiftX; ++i)
        std::rand();

    for (int x = 0; x < width; ++x) {
        map.push_back(std::vector<int>());
        for (int y = 0; y < height; ++y)
            map[x].push_back(48);

        int nextMove = std::rand() % 2;
        if (nextMove == 0 && lastHeight > 2 && currSecWidth > sectionWidth) {
            --lastHeight;
            currSecWidth = 0;
            sectionWidth = (std::rand() % (maxSecWidth - minSecWidth));
            sectionWidth += minSecWidth;
            isStart = true;
        }
        
        if (nextMove == 1 && lastHeight < height - 2 
                                && currSecWidth > sectionWidth) {
            ++lastHeight;
            currSecWidth = 0;
            sectionWidth = (std::rand() % (maxSecWidth - minSecWidth));
            sectionWidth += minSecWidth;
            isStart = true;
        }

        hasSpike = x > 3 ? std::rand() % 2 : 0;
        bool hasCoin = std::rand() % 10 > 5;
        
        ++currSecWidth;
        if (currSecWidth > sectionWidth 
                    && (lastHeight < height - 2 || lastHeight > 2)) {
            isEnd = true;
        }
        
        for (int y = lastHeight; y >= 0; --y) {
            // float spikeDist = std::sqrt(
            //                 (x - lastSpikeEnd.x) * (x - lastSpikeEnd.x ) + 
            //                 (y+1 - lastSpikeEnd.y) * (y+1 - lastSpikeEnd.y));
            float spikeDist = distance(lastSpikeEnd, {x, y+1});
            if (hasSpike && spikesLength <= 3) {
                if (y+1 < height && (map[x-1][y+1] == 107 || spikeDist > 2)) {
                    map[x][y+1] = 107;  // spike
                    ++spikesLength;
                    hasSpike = false;
                    lastSpikeEnd = {x, y + 1};
                }
            } 
            if (hasCoin) {
                int h = std::rand() % 3 + 2;
                if (y+h < height && distance(lastCoin, {x, y+h}) > 8) {
                    map[x][y+h] = 112;  // coin
                    lastCoin = {x, y + h};
                }
                hasCoin = false;
            }
            if (y == lastHeight) {
                map[x][y] = (currSecWidth + 1) % 2 ? 66 : 67;  // tile
            } else {
                map[x][y] = 49;  // tile ground
            }
            if (prevHeight < lastHeight && isStart) {
                map[x][y] = 65;  // begin tile
                isStart = false;
                spikesLength = 0;
                if (map[x-1][y-1] == 68)  // if end tile
                    map[x-1][y-1] = (currSecWidth + 1) % 2 ? 66 : 67;  // tile
            } else if (isEnd) {
                map[x][y] = 68;  // end tile
                isEnd = false;
                prevHeight = lastHeight;
            }
            hasSpike = false;
        }
    }
    m_height[seed] = lastHeight;
    return map;
}

std::vector<std::string> Generator::to_string(const GenResult& gr) {
    std::vector<std::string> str;
    int width = gr.size();
    int height = gr[0].size();
    for (int row = 0; row < height; ++row) {
        str.push_back("");
        for (int column = 0; column < width; ++column) {
            str[row] += static_cast<char>(gr[column][row]);
        }
        // str[row] += '\0';
    }
    return str;
}

template<typename T>
float Generator::distance(sf::Vector2<T> p1, sf::Vector2<T> p2) const {
    return std::sqrt((p2.x - p1.x) * (p2.x - p1.x) + 
                                (p2.y - p1.y) * (p2.y - p1.y));
}


void Generator::test() {
    int w = 60, h = 20;  // 60, 20, 1235, 0, 3, 6
    GenResult t = randomWalkTop(w, h, 1235, 0, 3, 6);
    for (int y = h; y >= 0; --y) {
        for (int x = 0; x < w; ++x) {
            std::cout << static_cast<char>(t[x][y]) << " ";
        }
        std::cout << std::endl;
    }
    // for (int x = 0; x < w; ++x) {
    //     for (int y = h - 1; y >= 0; --y) {
    //         std::cout << static_cast<char>(t[x][y]) << " ";
    //     }
    //     std::cout << std::endl;
    // }
}
