#include "zonemanager.h"
#include <RakNet/BitStream.h>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "samp/samp.h"
#include "plugin.h"

using json = nlohmann::json;

c_zone_manager::c_zone_manager() : nextZoneId(1000) {
    colorMap["rojo"] = 0x8000'00FF;
    colorMap["naranja"] = 0x8000'A5FF;
    colorMap["violeta"] = 0x80A0'20F0;
}

c_zone_manager::~c_zone_manager() {
    destroyAllZones();
}

void c_zone_manager::loadZones() {
    std::cout << "Voy a borrar las posible zonas que he añadido" << std::endl;
    destroyAllZones();
    zones.clear();
    nextZoneId = 1000;

    cpr::Response r = cpr::Get(cpr::Url{ "https://hidend.me/sapdapi/zones" });

    if (r.status_code != 200) {
        c_chat::get()->ref()->add_message(-1, "La web esta caida, @Hidend haz algo!");
        return;
    }

    try {
        json zonesData = json::parse(r.text);

        for (const auto& zoneData : zonesData) {
            Zone zone;
            zone.name = zoneData["name"];
            zone.minx = zoneData["minx"];
            zone.miny = zoneData["miny"];
            zone.maxx = zoneData["maxx"];
            zone.maxy = zoneData["maxy"];
            zone.color = zoneData["color"];

            if (zoneData.contains("ImageUrl") && !zoneData["ImageUrl"].is_null()) {
                zone.imageUrl = zoneData["ImageUrl"];
            }

            zone.zoneId = nextZoneId++;
            zones.push_back(zone);

            createGangZone(zone);
            std::cout << "Creo zona! " << std::endl;
        }

        c_chat::get()->ref()->add_message(-1, "Cargadas " + std::to_string(zones.size()) + " zonas! ");
    }
    catch (const std::exception& e) {
        std::cout << "Voy a borrar las posible zonas que he añadido " << e.what() << std::endl;
        c_chat::get()->ref()->add_message(-1, "Algo ha fallado, @Hidend :( haz algo");
    }
}

void c_zone_manager::destroyAllZones() {
    for (const auto& zone : zones) {
        destroyGangZone(zone.zoneId);
    }
}

void c_zone_manager::createGangZone(const Zone& zone) {
    uint32_t color = getColorForZone(zone.color);

    RakNet::BitStream bs;
    bs.Write<uint16_t>(zone.zoneId);
    bs.Write<float>(zone.minx);
    bs.Write<float>(zone.miny);
    bs.Write<float>(zone.maxx);
    bs.Write<float>(zone.maxy);
    bs.Write<uint32_t>(color);

    rakhook::emul_rpc(108, bs);
}

void c_zone_manager::destroyGangZone(uint16_t zoneId) {
    RakNet::BitStream bs;
    bs.Write<uint16_t>(zoneId);

    rakhook::emul_rpc(120, bs);
}

uint32_t c_zone_manager::getColorForZone(const std::string& colorName) {
    auto it = colorMap.find(colorName);
    if (it != colorMap.end()) {
        return it->second;
    }
    return 0xFFFF0000;
}