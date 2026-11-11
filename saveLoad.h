#pragma once
#include <vector>
#include <memory>
#include "Player.h"
#include "Room.h"
#include "Item.h"

void saveGame(const player& p, const Room* currentRoom, const std::vector<std::shared_ptr<item>>& inventory);
bool loadGame(player& p, Room*& currentRoom, const std::vector<std::unique_ptr<Room>>& allRooms, std::vector<std::shared_ptr<item>>& inventory);