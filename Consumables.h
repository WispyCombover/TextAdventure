#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Player.h"
#include "Item.h"

void useConsumable(player& p, const item& it);

bool drinkItem(player& p, std::vector<std::shared_ptr<item>>& inventory, const std::string& itemName);