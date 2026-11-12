#pragma once
#include <vector>
#include <memory>
#include "Room.h"

Room* buildWorld(std::vector<std::unique_ptr<Room>>& allRooms);