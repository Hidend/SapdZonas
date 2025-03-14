#pragma once
#include "models/Zone.h"
#include "samp/singleton.h"
#include <vector>
#include <unordered_map>

class c_zone_manager : public singleton<c_zone_manager> {
private:
    std::vector<Zone> zones;
    std::unordered_map<std::string, uint32_t> colorMap;
    uint16_t nextZoneId;

    void createGangZone(const Zone& zone);
    void destroyGangZone(uint16_t zoneId);
    uint32_t getColorForZone(const std::string& colorName);

public:
    c_zone_manager();
    ~c_zone_manager();

    void loadZones();
    void destroyAllZones();
};