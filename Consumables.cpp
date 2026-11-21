#include "Consumables.h"
#include "Utils.h"
#include <algorithm>
#include <iostream>

namespace {
    constexpr int kCoffeCupHeal = 10;       //HP fra full kaffekopp
}

void useConsumable(player& p, const item& it) {
    int amount = it.effectFlatBonus;
    for (int i = 0; i < it.effectDiceCount; ++i) {
        amount += rollDice(it.effectDiceSides);
    }

    if (!it.consumeText.empty()) {
        std::cout << it.consumeText << "\n";
    }

    switch (if.effect) {
        case itemEffect::healHp: {
            int oldHp = p.playerHP;
            p.playerHP = std::min(p.playerMaxHP, p.playerHP + amount);
            std::cout << "Du gjenvinner " << (p.playerHP - oldHp) << " HP! (" << p.playerHP << "/" << p.playerMaxHP << ")\n";
            break;
        }

        case itemEffect::restoreMana: {
            int oldMana = p.playerMana;
            p.playerMana = std::min(p.playerMaxMana, p.playerMana + amount);
            std::cout << "Du gjenvinner " << (p.playerMana - oldMana) << " Mana! (" << p.playerMana << "/" << p.playerMaxMana << ")\n";
            break;
        }

        case itemEffect::caffeineRush: {
            p.playerHP = std::min(p.playerMaxHP, p.playerHP + amount);
            p.addEffect(StatusType::caffeineRush, "Koffeinsjokk", 3, 2);
            std::cout << "Du får +" amount << " HP og [Koffeinsjokk] (+2 angrep/skade) i 3 runder!\n";
            break;
        }
        
        case itemEffect::sugarCrash: {
            p.playerHP = std::max(0, p.playerHP - amount);
            std::cout << "Du tar " << amount << " poison-damage! (" << p.playerHP << "/" << p.playerMaxHP << ")\n";
            break;
        }

        case itemEffect::none:
        default:
            std::cout << "Det skjer egentlig ingenting.\n";
            break;
    }
}

bool drinkItem(player& p, std::vector<std::shared_ptr<item>>& inventory, const std::string& itemName) {
    std::string wanted = toLowerNorwegian(itemName.empty() ? "kaffekopp" : itemName);

    auto it = std::find_if(inventory.begin(), inventory.end(), [&wanted](const std::shared_ptr<item>& i) { return toLowerNorwegian(i->name) == wanted; });
    if (it == inventory.end()) {
        std::cout << "Du har ingen '" << wanted << "' i sekken.\n";
        return false;
    }

    std::shared_ptr<item> selected = *it;

    if (selected->type == "consumable") {
        useConsumable(p, *selected);
        inventory.erase(it);
        return true;
    }

    if (selected->name == "kaffekopp") {
        if (!selected->isFilled) {
            std::cout << "Koppen er dessverre tom. Du må finne en kaffemaskin for å fylle den.\n";
            return false;
        }

        p.playerHP = std::min(p.playerMaxHP, p.playerHP + kCoffeCupHeal);
        emptyContainer(*selected);
        std::cout << "Du tar en slurk rykende varm kaffe. Du får tilbake " << kCoffeCupHeal << " HP! (HP: " << p.playerHP << "/" << p.playerMaxHP << ")\n";
        return true;
    }

    std::cout << "Du ikan ikke drikke " << selected->name << ".\n";
    return false;
}

