#pragma once
#include <vector>
#include <memory>
#include "Player.h"
#include "Room.h"
#include "Item.h"

bool handleCombat(player& p, Room*& currentRoom, std::vector<std::shared_ptr<item>>& inventory);