#include "Room.h"

Room::Room(std::string n, std::string d) : name(n), description(d) {}

void Room::addExit(const std::string& direction, Room* neighbor) {
    exits[direction].destination = neighbor;
}

void Room::addExit(std::initializer_list<std::string> directions, Room* neighbor) {
    for (const auto& direction : directions) {
        exits[direction].destination = neighbor;
    }
}

void Room::addHazard(const std::string& hazardName, int diceCount, int diceSides, const std::string& dmgType, int charges) {
    hazards.push_back({hazardName, diceCount, diceSides, dmgType, charges});
}