#include "CommandHandler.h"
#include "Combat.h"
#include "saveLoad.h"
#include <iostream>
#include <algorithm>

namespace {
    void handleMove(const std::string& direction, player& p, Room*& currentRoom) {
        if (currentRoom->exits.count(direction)) {
            p.lastRoom = currentRoom;
            currentRoom = currentRoom->exits[direction];
            std::cout << "\nDu går mot " << direction << ".\n";
        } else {
            std::cout << "Du kan ikke gå den veien.\n";
        }
    }

    void handleTake(const std::string& itemName, Room* currentRoom, std::vector<std::shared_ptr<item>>& inventory) {
        auto it = std::find_if(currentRoom->items.begin(), currentRoom->items.end(), [&itemName](const item& item) {
            return item.name == itemName;
        });

        if (it != currentRoom->items.end()) {
            inventory.push_back(std::make_shared<item>(*it));
            std::cout << "Du plukket opp: " << it->name << "\n";
            currentRoom->items.erase(it);
        } else {
            std::cout << "Den gjenstanden finnse ikke her.\n";
        }
    }

    void handleUse(const std::string& itemName, player& p, Room* currentRoom, std::vector<std::shared_ptr<item>>& inventory) {
        auto it = std::find_if(inventory.begin(), inventory.end(), [&itemName](const std::shared_ptr<item>& i) {
            return i->name == itemName;
        });

        if (it == inventory.end()) {
            std::cout << "Du har ingen '" << itemName << "' i sekken.\n";
            return;
        }

        std::shared_ptr<item> selectedItem = *it;

        if (selectedItem->type == "weapon" || selectedItem->type == "våpen") {
            p.equippedWeapon = selectedItem;
            std::cout << "Du utstyrte deg med: " << selectedItem->name << "!\n";
        }
        else if (selectedItem->name == "kaffekopp" || selectedItem->name == "kaffe" || selectedItem->type == "drink") {
            if (selectedItem->isFilled) {
                int healAmount = 10;   //Eller kanskje en rollDice(6) eller noe sånt er bedre
                p.playerHP = std::min(p.playerMaxHP, p.playerHP + healAmount);
                selectedItem->isFilled = false;
                std::cout << "Du tar en slurk rykende varm kaffe. Du føler deg våken og får tilbake " << healAmount << " HP! (HP: " << p.playerHP << "/" << p.playerMaxHP << ")\n";
            } else {
                std::cout << "Koppen er dessverre tom. Du må finne en kaffemaskin for å fylle den.\n";
            }
        }
        else if (selectedItem->type == "consumable" || selectedItem->type == "potion") {
            int healAmount = 15;
            p.playerHP = std::min(p.playerMaxHP, p.playerHP + healAmount);
            std::cout << "Du brukte " << selectedItem->name << " og fikk " << healAmount << " HP!\n";
            inventory.erase(it); // Fjern engangsgjenstanden etter bruk
        }
        else {
            std::cout << "Du vet ikke helt hvordan du skal bruke " << selectedItem->name << " her.\n";
        }
    }
}
bool processCommand(
    const std::string& input,
    player& p,
    Room*& currentRoom,
    const std::vector<std::unique_ptr<Room>>& allRooms,
    std::vector<std::shared_ptr<item>>& inventory
) {
    if (input == "avslutt" || input == "quit") {
        std::cout << "Takk for spillet!\n";
        return false;
    }

    // Deler input i verb (første ord) og mål (resten av linjen)
    std::string verb = input;
    std::string target = "";
    size_t spacePos = input.find(' ');
    
    if (spacePos != std::string::npos) {
        verb = input.substr(0, spacePos);
        target = input.substr(spacePos + 1);
    }

    // Håndter kommandoer
    if (verb == "gå" || verb == "go") {
        if (target.empty()) {
            std::cout << "Hvor vil du gå?\n";
        } else {
            handleMove(target, p, currentRoom);
        }
    }
    else if (verb == "ta" || verb == "take") {
        if (target.empty()) {
            std::cout << "Hva vil du ta?\n";
        } else {
            handleTake(target, currentRoom, inventory);
        }
    }
    else if (verb == "bruk" || verb == "use" || verb == "utstyr" || verb == "equip") {
        if (target.empty()) {
            std::cout << "Hva vil du bruke?\n";
        } else {
            handleUse(target, p, currentRoom, inventory);
        }
    }
    else if (verb == "drikk" || verb == "drink") {
        // Hvis spilleren bare skriver "drikk", antar vi at de mener kaffekoppen
        std::string drinkTarget = target.empty() ? "kaffekopp" : target;
        handleUse(drinkTarget, p, currentRoom, inventory);
    }
    else if (input == "se" || input == "look") {
        std::cout << "[]" << currentRoom->name << "]\n" << currentRoom->description << "\n";
        if (!currentRoom->items.empty()) {
            std::cout << "Du ser følgende gjenstander her:\n";
            for (const auto& it : currentRoom->items) {
                std::cout <<"- " << it.name << "\n";
            }
        }
    }
    else if (input == "inventar" || input == "sekk" || input == "i") {
        if (inventory.empty()) {
            std::cout << "Sekken din er tom.\n";
        } else {
            std::cout << "I sekken har du:\n";
            for (const auto& it : inventory) {
                std::cout << "- " << it->name << (it->isFilled ? " (full)" : "") << "\n";
            }
        }
    }
    else if (input == "lagre" || input == "save") {
        saveGame(p, currentRoom, inventory);
    }
    else if (input == "last" || input == "load") {
        if (loadGame(p, currentRoom, allRooms, inventory))
            std::cout << "Spillet ble lastet inn.\n";
        else    std::cout << "Fant ingen lagringsfil.\n";
    }
    else if (input == "fyll kopp" || input == "bruk kaffemaskin") {
        if (currentRoom->name != "Korridoren mot øst") {
            std::cout << "Det er ingen kaffemaskin her.\n";
        } else {
            //Sjekker om spilleren har en kaffekopp i inventaret
            auto it = std::find_if(inventory.begin(), inventory.end(), [](const std::shared_ptr<item>& i) {
                return i->name == "kaffekopp";
            });

            if (it != inventory.end()) {
                if ((*it)->isFilled) {
                    std::cout << "Koppen er allerede full av rykende fersk kaffe!\n";
                } else {
                    (*it)->isFilled = true;
                    (*it)->description = "En kaffekopp fylt med rykende fersk, varm og etsende trakterkaffe! (1d2 bludgeoning + 1d4 acid).\n";
                    (*it)->extraDiceSides = 4;
                    (*it)->extraDiceCount = 1;
                    (*it)->extraDamageType = "Acid";

                    std::cout << "*BRRrrrrrzzzzzt* Du fyller kaffekoppen med beksvart, rykende varm kontorkaffe! Den lukter etsende, men også oppfriskende.\n";
                }
            } else {
                std::cout << "Du har ingen kaffekopp å fylle. Kanskje du finner en på et møterom eller noe?\n";
            }
        }
    }
    
    //HJELPEMENY
    else if (input == "hjelp" || input == "help") {
        std::cout << " <TITTEL PÅ SPILL HER> er et humoristisk teksteventyr bygget på lesten av klassisk fantasy og med 'lette' D&D-regler.\n";
        std::cout << " Det fungerer slik at etter hvert som du navigerer rundt i spillverdenen får du opp en beskrivelse av hva du ser.\n";
        std::cout << " Det vil også stå hvor du potensielt kan bevege deg videre.\n";
        std::cout << " ---------------------------------------------------------------------------------------------------------------- \n";
        std::cout << " Måten du interagerer med spillverdenen på er via tekstkommandoer av typen: <verb> <objekt> \n";
        std::cout << " For eksempel: ta kopp\n\n";
        std::cout << " I tillegg har du noen faste kommandoer som kan hjelpe:\n\n";
        std::cout << " Inventory/inventory/inventar/inv/i/se sekk - åpner inventaret og lister ut innhold\n";
        std::cout << " Avslutt/quit/avslutt - avslutter spillet.\n";
        std::cout << " Save/save/lagre - Lagrer spillet der du står.\n";
        std::cout << " Load/load/last - Laster inn tidligere lagret spill.\n";
    }
    //UKJENT KOMMANDO
    else {
        std::cout << "Ukjent kommando. Prøv 'gå [retning]', 'ta [gjenstand]', 'inventory', eller 'avslutt'.\n";
    }
    return true;
}