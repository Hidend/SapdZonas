#pragma once
#include <string>
#include <cstdint>

class Zone {
public:
    std::string name;
    float minx;
    float miny;
    float maxx;
    float maxy;
    std::string color;
    std::string imageUrl;
    uint16_t zoneId;
};