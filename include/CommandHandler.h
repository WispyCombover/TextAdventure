#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Player.h"
#include "Room.h"
#include "Item.h"

void printRoomDescription(const Room* room);

bool processCommand(
    const std::string& input,
    player &p,
    Room*& currentRoom,
    const std::vector<std::unique_ptr<Room>>& allRooms,
    std::vector<std::shared_ptr<item>>& inventory
);