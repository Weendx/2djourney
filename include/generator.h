#pragma once
#include <string>
#include <vector>


using GenResult = std::vector<std::vector<int>>;

class Generator {
 public:
    GenResult randomWalkTop(const int& width, const int& height, 
            const int& seed, const int& shiftX,
            const int& minSectionWidth, const int& maxSectionWidth);
    std::vector<std::string> to_string(const GenResult& gr);
    void test();
};
