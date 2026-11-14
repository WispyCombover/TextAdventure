#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <initializer_list>
#include "Item.h"
#include "Monster.h"
#include "NPC.h"

struct Exit
{
    Room *destination = nullptr;
    bool hasDoor = false;            //Er det en dør, eller en passasje?
    bool isOpen = true;              //Er døren åpen akkurat nå?
    bool isLocked = false;           //Er døren låst?
    std::string keyName = "";        //Hva trengs for å låse opp døren?
    std::string description = "";    //Tekst som vises når "se dør"
};

class Room {
    public:
    std::string name;
    std::string description;
    std::map<std::string, Exit> exits;  //Peker til tilstøtende rom
    std::vector<item> items;
    std::unique_ptr<monster> monster;  //nullptr for rom der det ikke er monstre
    std::vector<std::unique_ptr<NPC>> npcs;
    std::map<std::string, std::string> scenery;

    Room(std::string n, std::string d);

    void addExit(const std::string& direction, Room* neighbor);
    void addExit(std::initializer_list<std::string> directions, Room* neighbor);
};