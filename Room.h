#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <initializer_list>
#include "Item.h"
#include "Monster.h"
#include "NPC.h"

class Room {
public:
    struct Door {
        std::string id;
        bool isOpen = false;
        bool isLocked = false;
        std::string keyName = "";
    };

    struct Exit {
        Room* destination = nullptr;
        std::shared_ptr<Door> door;
        std::string description = "";
    };

    struct Hazard {
        std::string name;
        int damageDiceCount = 1;
        int damageDiceSides = 4;
        std::string damageType = "Piercing";
        int charges = 3;
    };
    
    std::string name;
    std::string description;
    std::map<std::string, Exit> exits;  //Peker til tilstøtende rom
    std::vector<item> items;
    std::vector<Hazard> hazards;
    std::unique_ptr<monster> occupant;  //nullptr for rom der det ikke er monstre
    std::vector<std::unique_ptr<NPC>> npcs;
    std::map<std::string, std::string> scenery;
    std::map<std::string, std::string> hiddenItems;  //key = scenery-navn (feks 'sofa'), value = item-navn (feks "inntørket vestlandslefse")
    int discoveryXP = 5;        //Standard XP for å oppdage et rom
    bool visited = false;       //Settes til true ved første besøk

    Room(std::string n, std::string d);

    void addExit(const std::string& direction, Room* neighbor);
    void addExit(std::initializer_list<std::string> directions, Room* neighbor);
    void addHazard(const std::string& hazardName, int diceCount, int diceSides, const std::string& dmgType, int charges);
};