#pragma once
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include "Room.h"
#include "Player.h"
#include "NPC.h"

struct ExitBlueprint {
    std::string direction;              //nord, sør, sala segfault
    std::string targetRoomId;           //ID of destination room
    std::string doorId = "";            //Shared door ID (empty = no door)
    std::string inspectText = "";       //Optional. Flavourtext
    bool appendTargetAddress = false;   //Automatically sets memory address
};

struct DoorBlueprint {
    std::string id;
    bool locked = false;
    std::string keyName = "";
};

struct SceneryBlueprint {
    std::vector<std::string> aliases;   // Whiteboard, tavle -> mapper begge til samme text
    std::string description;
    std::string hiddenItem = "";
    std::string addressTargetId = "";   // Valgfri rom-ID hjelper for minneadresser
};

struct RoomBlueprint {
    std::string id;
    std::string name;
    std::string description;
    int discoveryXP = 5;
    bool startsVisited = false;

    std::vector<ExitBlueprint> exits;
    std::vector<std::string> items;
    std::vector<SceneryBlueprint> scenery;

    //Encounters og NPCer
    int randomMonsterLevel = 0;         // 0 = ingen monstre
    int fixedMonsterLevel = 0;
    std::string fixedMonsterName = "";
    std::vector<std::function<std::unique_ptr<NPC>(const player&)>> npcFactories;
};

const std::vector<DoorBlueprint>& getFloor1Doors();
const std::vector<RoomBlueprint>& getFloor1Rooms();