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