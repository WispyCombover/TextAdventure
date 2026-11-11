#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <initializer_list>
#include "Item.h"
#include "Monster.h"

class Room {
    public:
    std::string name;
    std::string description;
    std::map<std::string, Room*> exits;  //Peker til tilstøtende rom
    std::vector<item> items;
    std::unique_ptr<monster> monster;  //nullptr for rom der det ikke er monstre

    Room(std::string n, std::string d);

    void addExit(const std::string& direction, Room* neighbor);
    void addExit(std::initializer_list<std::string> directions, Room* neighbor);
};