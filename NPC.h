#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Item.h"
#include "Player.h"

struct dialogueOption {
    std::string playerText;    //Dialogalternativ for spiller
    int nextNodeIndex;         //Neste node på dialogtreet
    bool opensShop = false;    //Sett til true for åpne butikkmenyen
};

struct dialogueNode {
    std::string npcText;
    std::vector<dialogueOption> options;
};

struct NPC {
    std::string name;
    std::string description;
    
    //Dialogtre
    std::vector<dialogueNode> dialogueTree;
    int currentNodeIndex = 0;
    
    //Merchant egenskaper
    bool isMerchant = false;
    std::vector<item> shopInventory;
};

// Genererer NPCer her, definerer dem i NPC.cpp
std::unique_ptr<NPC> createFirewallGuru(const player& p);
std::unique_ptr<NPC> createHRRep(const player& p);