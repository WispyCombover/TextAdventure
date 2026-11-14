#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Player.h"
#include "Room.h"
#include "Monster.h"
#include "Item.h"

enum class spellSpecial {
    None,
    Sudo,           //Neste spell koster 0 mana og kan ikke fumble
    KillNine,       //Dreper svake monstre øyeblikkelig
    RmRf,           //Massiv skade, men sletter inventaret (unntatt quest-items)
    Escape,         //Flykter tilbake til forrige rom (ssh)
    Identify,       //Viser monsterets stats (ls -la)
    Ping,           //Sjekker nabo-rom for monstre
    Grep            //Lister opp ting i rommet
};

struct spell {
    std::string name;
    std::string description;
    int level = 1;
    int manaCost = 0;

    int damageDiceCount = 0;
    int damageDiceSides = 0;
    int healDiceCount = 0;
    int healDiceSides = 0;
    int acReduction = 0;        //Reduserer monster AC
    int targetDamageBoost = 0;  //Buff til monster
    int recoilDamage = 0;       //Skade på caster

    bool needsTarget = false;
    spellSpecial special = spellSpecial::None;

    std::string castMessage;
    std::string fizzleMessage;  //Når man ikke har nok mana
};

enum class castResult {
    NotFound,
    NoSpellbook,
    LevelTooLow,
    NoMana,
    NoTarget,
    Cast,           //Great success! Runden er brukt
    Fumbled,        //Segfault! Runden er brukt
    Escaped,        //Spilleren flyktet
    PlayerDied
};

const std::vector<spell>& spellList();
const spell* findSpell(const std::string& name);
bool hasSpellbook(const std::vector<std::shared_ptr<item>>& inventory);

void listSpells(const player& p, const std::vector<std::shared_ptr<item>>& inventory);

//Target kan være nullptr utenfor kamp
castResult castSpell(
    player& p,
    const std::string& spellName,
    monster* target,
    const Room* currentRoom,
    std::vector<std::shared_ptr<item>>& inventory
);

inline bool spellUsedTurn(castResult r) {
    return r == castResult::Cast || r == castResult::Fumbled || r == castResult::Escaped || r == castResult::PlayerDied;
}

