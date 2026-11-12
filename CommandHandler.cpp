#include "CommandHandler.h"
#include "Combat.h"
#include "saveLoad.h"
#include "NPC.h"
#include "Utils.h"
#include "Room.h"
#include "WorldBuilder.h"
#include "Player.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <limits>

void buyFromMerchant(NPC& npc, player& p) {
    while (true) {
        std::cout << "\n--- " << npc.name << "s Butikk ---\n";
        std::cout << "Din saldo: " << formatDataCurrency(p.dataBalanceKB) << "\n\n";

        if (npc.shopInventory.empty()) {
            std::cout << "Butikken er helt tom! Kom tibake en annen gang.\n";
            return;
        }

        for (size_t i = 0; i < npc.shopInventory.size(); ++i) {
            const auto& item = npc.shopInventory[i];
            std::cout << (i + 1) << ". " << item.name << " (" << item.value << " " << item.currency << ") - " << item.description << "\n";  
        }
        std::cout << "0. Forlat butikken\n";
        std::cout << "Velg vare (tast inn tallverdi): > ";

        if (choice == 0) {
            std::cout << "Du avslutter handelen.\n";
            break;
        }

        if (choice > 0 && static_cast<size_t>(choice) <= npc.shopInventory.size()) {
            item selectedItem = npc.shopInventory[choice - 1];
            long long priceInKB = convertToKB(selectedItem.value, selectedItem.currency);

            if (p.dataBalanceKB >= priceInKB) {
                p.dataBalanceKB -= priceInKB;
                p.inventory.push_back(selectedItem);
                npc.shopInventory.erase(npc.shopInventory.begin() + (choice - 1));
            } else {
                std::cout << "Du har ikke nok båndbredde! Du mangler " << formatDataCurrency(priceInKB - p.dataBalanceKB) << ".\n"; 
            }
        } else {
            std::cout << "Ugyldig varenummer.\n";
        }
    }
}

void sellToMerchant(NPC& npc, player& p) {
    while (true) {
        std::cout << "\n--- Selg ti " << npc.name << " ---\n";
        std::cout << "Din saldo: " << formatDataCurrency(p.dataBalanceKB) << "\n\n";

        if (p.inventory.empty()) {
            std::cout << "Ryggsekken din er helt tom!\n";
            return;
        }

        for (size_t i = 0; i < p.inventory.size(); ++i) {
            const auto& item = p.inventory[i];
            long long baseValueKB = convertToKB(item.value, item.currency);
            long long sellPRiceKB = baseValueKB / 2; //50% innbyttepris

            std::cout << (i + 1) << ". " << item.name;
            if (item.isQuestItem || sellPriceKB <= 0) {
                std::cout << " [Kan ikke selges]\n";
            } else {
                std::cout << " (Selg for: " << formatDataCurrency(sellPriceKB) << ")\n";
            }
        }
        std::cout << "0. Tilbake til butikkmeny\n";
        std::cout << "Velg vare å selge: > ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ugyldig valg. Skriv et tall.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 0) break;

        if (choice > 0 && static_cast<size_t>(choice) <= p.inventory.size()) {
            item selectedItem = p.inventory[choice - 1];
            long long sellPriceKB = convertToKB(selectedItem.value, selectedItem.currency) / 2;

            if (selectedItem.isQuestItem) {
                std::cout << npc.name << ": \"Den der ser altfor viktig ut til at jeg tør å ta den.\"\n";
                continue;
            }
            if (sellPriceKB <= 0) {
                std::cout << npc.name << ": \"Det der er bare søppel! Jeg gir deg ikke én byte for det!\"\n";
                continue;
            }

            //Transaksjon
            p.dataBalandeKB += sellPriceKB;
            npc.shopInventory.push_back(selectedItem);
            p.inventory.erase(p.inventory.begin() + (choice - 1));

            std::cout << "Du solgte " << selectedItem.name << " for " << formatDataCurrency(sellPriceKB) << "!\n";
        } else {
            std::cout << "Ugyldig varenummer.\n";
        }
    }
}

void handleShop(NPC& npc, player& p) {
    while (true) {
        std::cout << "\n=== " << npc.name << "s Båndbredde-børs ===\n";
        std::cout << "Din saldo: " << formatDataCurrency(p.dataBalanceKB) << "\n";
        std::cout << "1. Kjøp utstyr\n";
        std::cout << "2. Selg gjenstander\n";
        std::cout << "0. Forlat butikken\n";
        std::Cout << "Valg: > ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore
        }
    }
}

void printRoomDescription(const Room* room) {
    std::cout << "\n------------------------------------------\n";
    std::cout << "[" << room->name << "]\n";
    std::cout << room->description << "\n";

    if (!room->items.empty()) {
         std::cout << "\nDu ser følgende gjenstander her:\n";
        for (const auto& item : room->items) {
            std::cout << "- " << item.name << "\n";
        }
    }
    if (!room->npcs.empty()) {
        std::cout << "\nDu ser følgende skikkelser her:\n";
        for (const auto& npc : room->npcs) {
            std::cout << "- " << npc->name << "\n";
        }
    }
}

void handleTalk(const std::string& targetName, Room* currentRoom, player& p) {
    if (targetName.empty()) {
        std::cout << "Hvem vil du snakke med?\n";
        return;
    }

    std::string cleanTarget = toLowerNorwegian(targetName);

    if (cleanTarget.rfind("med ", 0) == 0) {
        cleanTarget = cleanTarget.substr(4);
    }

    NPC* foundNPC = nullptr;
    for (auto& npc : currentRoom->npcs) {
        if (toLowerNorwegian(npc->name).find(cleanTarget) != std::string::npos) {
            foundNPC = npc.get();
            break;
        }
    }

    if (!foundNPC) {
        std::cout << "Finner ingen her so heter '" << targetName << "'.\n";
        return;
    }

    if (foundNPC->dialogueTree.empty()) {
        std::cout << foundNPC->name << " har ingenting å si.\n";
        return;
    }

    int currentNode = 0;
    while (currentNode != -1 && static_cast<size_t>(currentNode) < foundNPC->dialogueTree.size()) {
        const auto& node = foundNPC->dialogueTree[currentNode];
        std::cout << "\n[" << foundNPC->name << "]: \"" <<node.npcText << "\"\n";

        if (node.options.empty()) {
            break;
        }

        for (size_t i = 0; i < node.options.size(); ++i) {
            std::cout << (i + 1) << ". " << node.options[i].playerText << "\n";
        }
        std::cout << "Ditt svar (tall): > ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Skriv inn et gyldig tall.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (chosenOption.opensShop && foundNPC->isMerchant) {
            std::cout << "\n[" << foundNPC->name << "]: \"" << foundNPC->dialogueTree[chosenOption.nextNodeIndex].npcText << "\"\n";
            handleShop(*foundNPC, p);
            break;
        }
        currentNode = chosenOption.nextNodeIndex;
    } else {
        std::cout << "Ugyldig valg.\n";
    }
}

namespace {
    void handleMove(const std::string& direction, player& p, Room*& currentRoom) {
        std::string cleanDir = toLowerNorwegian(direction);

        if (currentRoom->exits.count(direction)) {
            p.lastRoom = currentRoom;
            currentRoom = currentRoom->exits[direction];
            std::cout << "\nDu går mot " << direction << ".\n";
            printRoomDescription(currentRoom);
        } else {
            std::cout << "Du kan ikke gå den veien.\n";
        }
    }

    void handleTake(const std::string& itemName, Room* currentRoom, std::vector<std::shared_ptr<item>>& inventory) {
        std::string cleanInput = toLowerNorwegian(itemName);

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
        std::string cleanInput = toLowerNorwegian(itemName);
 
        auto it = std::find_if(inventory.begin(), inventory.end(), [&itemName](const std::shared_ptr<item>& i) {
            return i->name == itemName;
        });

        if (it == inventory.end()) {
            std::cout << "Du har ingen '" << itemName << "' i sekken.\n";
            return;
        }

        std::shared_ptr<item> selectedItem = *it;
        std::string lowerName = toLowerNorwegian(selectedItem->name);
        std::string lowerType = toLowerNorwegian(selectedItem->type);

        if (lowerType == "weapon" || lowerType == "våpen") {
            p.equippedWeapon = selectedItem;
            std::cout << "Du utstyrte deg med: " << selectedItem->name << "!\n";
        }
        else if (lowerName == "kaffekopp" || lowerName == "kaffe" || lowerName == "drink") {
            if (selectedItem->isFilled) {
                int healAmount = 10;   //Eller kanskje en rollDice(6) eller noe sånt er bedre
                p.playerHP = std::min(p.playerMaxHP, p.playerHP + healAmount);
                selectedItem->isFilled = false;
                std::cout << "Du tar en slurk rykende varm kaffe. Du føler deg våken og får tilbake " << healAmount << " HP! (HP: " << p.playerHP << "/" << p.playerMaxHP << ")\n";
            } else {
                std::cout << "Koppen er dessverre tom. Du må finne en kaffemaskin for å fylle den.\n";
            }
        }
        else if (lowerType == "consumable" || lowerType == "potion") {
            int healAmount = 15;
            p.playerHP = std::min(p.playerMaxHP, p.playerHP + healAmount);
            std::cout << "Du brukte " << selectedItem->name << " og fikk " << healAmount << " HP!\n";
            inventory.erase(it); // Fjern engangsgjenstanden etter bruk
        }
        else {
            std::cout << "Du vet ikke helt hvordan du skal bruke " << selectedItem->name << " her.\n";
        }
    }

    void handleFill(const std::string& targetName, Room* currentRoom, std::vector<std::shared_ptr<item>>& inventory) {
        if (targetName.empty()) {
            std::cout << "Hva vil du fylle?\n";
            return;
        }
        std::string cleanTarget = toLowerNorwegian(targetName);
        
        auto it = std::find_if(inventory.begin(), inventory.end(), [&cleanTarget](const std::shared_ptr<item>& i) {
        return toLowerNorwegian(i->name) == cleanTarget;
        });

        if (it == inventory.end()) {
            std::cout << "Du har ingen '" << targetName << "' i sekken.\n";
            return;
        }

        std::shared_ptr<item> container = *it;

        if (container->isFilled) {
            std::cout << container->name << " er allerede full!\n";  //Kan være kult om den sier hva den er full av, og gir valget om å bytte innhold
            return;
        }
        //Sjekk hvilken kilde til væske som finnes i rommet
        std::string roomLower = toLowerNorwegian(currentRoom->name);
        
        if (roomLower == "korridoren mot øst") { //Er ikke forelsket i dette romnavnet ... vurdere å bytte
            container->isFilled = true;
            container->description = "Fylt med lett svidd, glovarm, beksvart og etsende trakterkaffe! (1d2 bludgeoning + 1d4 acid).\n";
            container->extraDiceSides = 4;
            container->extraDiceCount = 1;
            container->extraDamageType = "Acid";

            std::cout << "*BRRrrrzzzzt* Du fyller " << container->name << " med beksvart, rykende varm kontorkaffe! Den lukter etsende, men også oppfriskende.\n";
        }
        else if (roomLower == "toalettet" || roomLower == "kjøkkenet") {
            container->isFilled = true;
            container->description = "Fylt med Fylt med lunkent springvann.\n";
            container->extraDiceSides = 0;
            container->extraDiceCount = 0;
            container->extraDamageType = "";

            std::cout << "Du fyller " << container->name << " med friskt vann fra springen.\n";
        } else {
            std::cout << "Det er ingenting å fylle " << container->name << " med her.\n";
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
    std::string lowerInput = toLowerNorwegian(input);

    if (lowerInput == "avslutt" || lowerInput == "quit") {
        std::cout << "Takk for spillet!\n";
        return false;
    }

    // Deler input i verb (første ord) og mål (resten av linjen)
    std::string verb = lowerInput;
    std::string target = "";
    size_t spacePos = input.find(' ');
    
    if (spacePos != std::string::npos) {
        verb = toLowerNorwegian(input.substr(0, spacePos));
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
    else if (verb == "fyll" || verb == "fill") {
        if (target.empty()) {
            std::cout << "Hva vil du fylle?\n";
        } else {
            handleFill(target, p, currentRoom, inventory);
        }
    }
    else if (verb == "drikk" || verb == "drink") {
        // Hvis spilleren bare skriver "drikk", antar vi at de mener kaffekoppen
        std::string drinkTarget = target.empty() ? "kaffekopp" : target;
        handleUse(drinkTarget, p, currentRoom, inventory);
    }
    else if (verb == "se" || verb == "look") {
        if (target.empty()) {
            printRoomDescription(currentRoom);
        } else {
            //Et bra sted for en 'inspect'-funskjon?
            handleUse(target, p, currentRoom, inventory);
        }
    }
    else if (lowerInput == "inventar" || lowerInput == "sekk" || lowerInput == "i") {
        if (inventory.empty()) {
            std::cout << "Sekken din er tom.\n";
        } else {
            std::cout << "I sekken har du:\n";
            for (const auto& it : inventory) {
                std::cout << "- " << it->name << (it->isFilled ? " (full)" : "") << "\n";
            }
        }
    }
    else if (lowerInput == "lagre" || lowerInput == "save") {
        saveGame(p, currentRoom, inventory);
    }
    else if (lowerInput == "last" || lowerInput == "load") {
        if (loadGame(p, currentRoom, allRooms, inventory)) {
            std::cout << "Spillet ble lastet inn.\n";
            printRoomDescription(currentRoom);
        } else {
            std::cout << "Fant ingen lagringsfil.\n";
        }
    }
    else if (verb == "snakk" || verb == "talk" || verb == "prat" || verb == "snakk") {
        if (target.empty()) {
            std::cout << "Hvem vil du snakke med?\n";
        } else {
            handleTalk(target, currentRoom);
        }
    }
    
    //HJELPEMENY
    else if (lowerInput == "hjelp" || lowerInput == "help" || lowerInput == "?") {
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
    else if (lowerInput == "panikk" || lowerInput == "panic") {
        std::cout << "Du trosser omslaget på din trofaste kopi av The Hitchhiker's Guide to the Galaxy, og lar panikken velte over deg!\n";
        std::cout << "De neste femten minuttene tilbringer du hulkende i nærmeste hjørne.\n";
        std::cout << "Tårevåt og en smule snørrete bestemmer du deg deretter for å stramme deg opp, og for å gi det et nytt forsøk.\n";
    }
    //UKJENT KOMMANDO
    else {
        std::cout << "Ukjent kommando. Prøv 'gå [retning]', 'ta [gjenstand]', 'inventory', eller 'avslutt'.\n";
    }
    return true;
}