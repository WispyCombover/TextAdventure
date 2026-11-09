#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <windows.h>
#include <random>
#include <initializer_list>

//Definerer spillobjekt (gjenstander i verdenen)
struct item {
    std::string name;
    std::string description;
    int value;
};

struct monster {
    std::string name;
    std::string description;
    int hp;
    int maxHp;
    int attackDamage;
    std::string attackMessage;
};

// Definererr rom 
class Room {
    public:
        std::string name;
        std::string description;
        std::map<std::string, Room*> exits; //Peker til tilstøtende rom
        std::vector<item>items;
        std::unique_ptr<monster> monster; //NULLPTR for rom uten monster

        Room(std::string n, std::string d) : name(n), description(d) {}
        
        void addExit(const std::string& direction, Room* neighbor) {
            exits[direction] = neighbor;
        }

        void addExit(std::initializer_list<std::string> directions, Room* neighbor) {
            for (const auto& direction : directions) {
                exits[direction] = neighbor;
            }
        }
};

int main() {
    SetConsoleOutputCP(CP_UTF8); // Sørger for å støtte norske tegn
    SetConsoleCP(CP_UTF8); // Sørger for å støtte norske tegn

    //1. VERDENSBYGGING (DM-ing)
    //Prøver å bruke unique_ptr for å håndtere minne automatisk - manuell minnehåndtering er fremdeles innmari skummelt!
    auto spawnRoom = std::make_unique<Room>(
        "Møterommet",
        "Du står alene på møterommet. De andre gikk for noen minutter siden, men du ble stående igjen. Det lukter stramt av gammel svette og kald kaffe.\nEt svakt lyst kommer gjennom de frostede vinduene ut i korridoren utenfor."     
    );

    auto corridor = std::make_unique<Room>(
        "Korridoren",
        "En lang, smal korridor med falmede tapeter og knirkende gulvplanker. Det er flere dører til tilstøtende møterom langs veggene, men lyser rødt for å indikere at de er opptatt.\nKorridoren fortsetter mot VEST og mot ØST, og du kan også gå tilbake gjennom DØR til møterommet du nettopp forlot."
    );

    auto corridorWest = std::make_unique<Room>(
        "Korridoren mot vest",
        "Du kommet til et hjørne som går videre mot NORD og inn i kontorlandskapet. Rett foran deg er der en dør. På veggen ved siden av døren ser du en kortleser med et kodepanel." //Trenger logikk for å håndtere lesing av nøkkelkort samt inntasting av kode
    );

    auto corridorEast = std::make_unique<Room>(
        "Korridoren mot øst",
        "Du kommer til et lite hjørne av korridoren. Det er stille her, men du kan høre lyden av dempet prat fra at av de nærmeste møterommene. \n Her ser du også en liten sittegruppe med noen stoler og et bord, og på et lite bord i hjørnet står en gammel kaffemaskin. \nDu kan gå tilbake mot VEST, eller gjennom en dør mot NORD." //Kaffemaskinen må kunne brukes til å fylle koppen med varm kaffe (Acid attack, AOE 1d4)
    );

    //Koble rommene sammen (retning, peker)
    spawnRoom->addExit({"DØR"}, corridor.get());
    corridor->addExit({"DØR"}, spawnRoom.get());
    corridor->addExit({"ØST", "øst", "EAST", "east", "E", "e", "Ø", "ø", "East", "Øst"}, corridorEast.get());
    corridor->addExit({"WEST"}, corridorWest.get());
    corridorEast->addExit({"VEST", "WEST", "vest", "west", "V", "v", "W", "w", "West", "Vest"}, corridor.get());

    // Legg til en item
    spawnRoom->items.push_back(item{"kaffekopp", "En mye brukt kaffekopp som noen har glemt igjen. Innsiden er misfarget av gammel kaffe. (Verdi 2CP)"});
    spawnRoom->items.push_back(item{"penn", "En vanlig BIC-penn som har tørket inn. Improvisert stikkvåpen (1d4 skade)"});

    //2. SPILLER-TILSTAND
    Room* currentRoom = spawnRoom.get();
    std::vector<item>inventory;

    int playerHP = 20;  // Modifiseres av 'databrus' (healing) og andre gjenstander
    int playerMaxHP = 20;
    int playerBaseDamage = 2;  // Modifiseres av våpen og gjenstander

    bool running = true;

    std::cout << "=== VELKOMMEN til MIMIRS HEMMELIGE DUNGEON CRAWLER ===\n\n";
    std::cout << "------------------------------------------------------\n\n";
    std::cout << "Det siste møtet kom med fryktelige avsløringer og overraskelser! \n";
    std::cout << "Deadlinemonsteret™ har stukket av med krypteringsnøklene, og Mimir har sendt deg ut for å hente dem tilbake. \n"; 
    std::cout << "Du må navigere gjennom kontorlandskapet, samle gjenstander og bekjempe monstre for å finne nøklene og redde Mimir fra en sikker død og evig segfault.\n";
    std::cout << "Lykke til, vår alles fremtid ahenger av deg og ditt heltemot!\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "Skriv 'hjelp' for å se tilgjengelige kommandoer.\n\n";

    //3. GAME-LOOP
    while (running) {
        //Sjekk om rommet inneholder et monster
        if (currentRoom->monster && currentRoom->monster->hp > 0) {
            monster* m = currentRoom->monster.get();
            std::cout << "\n!!! SKREKK OG GRU !!!\n";
            std::cout << "Du møter på " << m->name << "!\n";
            std::cout << m->description << " (HP: " << m->hp << "/" << m->maxHp << ")\n";
            std::cout << "-----------------------------------------------------\n";
            std::cout << "Hva gjør du? [angrip / flykt]\n";

            std::string combatInput;
            std::getline(std::cin, combatInput);
            // std::transform(combatInput.begin(), combatInput.end(), combatInput.begin(), ::tolower);

            if (combatInput == "angrip" || combatInput == "slå") {
                //Spilleren angriper monsteret
                //Sjekker om spilleren har et våpen i inventaret som kan øke skaden
                int damageDealt = playerBaseDamage;

                //Sjekk etter våpen
                auto hasWeapon = std::find_if(inventory.begin(), inventory.end(), [](const item& i) {
                    return i.name == "penn";});
                    if (hasWeapon != inventory.end()) {
                        damageDealt +=2; //Penn gjør 1d4, må lage terninglogikk! Fast +2 enn så lenge.
                        std::cout << "Du stikker febrilsk med den inntørkede pennen!\n";
                    } else {
                        std::cout << "Du slår løs med knyttnevene!\n";
                    }

                    m->hp -= damageDealt;
                    std::cout << "Du gjorde " << damageDealt << " skade på " << m->name << ".\n";

                    //Sjekk om moonsteret lever
                    if (m->hp <= 0) {
                        std::cout << "Du beseiret " << m->name << "! Veien videre er klar.\n";
                        currentRoom->monster = nullptr; // Fjerner monsteret fra rommet
                        continue; //Tilbake til room descritpiojn
                    }
                }
                else if (combatInput == "flykt") {
                    //Flykt tilbake til et trygt rom
                    std::cout << "Du får panikk, snur på hælen og løper tilbake til start!\n";
                    currentRoom = spawnRoom.get(); //Trenger å lagre lastRoom - må lage logikk for det
                    continue;
                }
                else {
                    std::cout << "Du nøler, og det koster deg dyrt...\n";
                }

                //Monsterets tur til å angripe (om det lever)
                if (m->hp > 0) {
                    std::cout << "\n" << m-> name << " " << m->attackMessage << "\n";
                    playerHP -= m->attackDamage; //Trenger noe armor-logikk for senere
                    std::cout << "Du tok " << m->attackDamage << " skade! Din HP: " << playerHP << "/" << playerMaxHP << "\n";

                    if (playerHP <= 0) {
                        std::cout << "\n=== GAME OVER ===\n";
                        std::cout << "Deadlinemonsteret vant denne gangen. Mimir er skuffet, og logger deg ut!\n";
                        running = false;
                    } 
                }
                continue; //Hopper over den vanlige rom-utforskningen når det er kamp
        }
        std::cout << "\n---------------------------------------\n";
        std::cout << "[" << currentRoom->name << "]\n";
        std::cout << currentRoom->description << "\n";
    
    if (!currentRoom->items.empty()) {
        std::cout << "Du ser følgende gjenstander her:\n";
        for (const auto& item : currentRoom->items) {
            std::cout << "- " << item.name << "\n";
        }
    }

    // Vent på kommando fra spilleren
    std::cout << "\nHva vil du gjøre? > ";
    std::string input;
    std::getline(std::cin, input);

    //Enkel parsing: konverter til små bokstaver for å unngå problemer med Caps-lock - denne sliter med norske tegn, må se etter fiks
    // std::transform(input.begin(), input.end(), input.begin(), ::tolower);

    //Kommandotolk (parser)
    if (input == "avslutt" || input == "exit" || input == "quit") {
        running = false;
        std::cout << "Spillet avsluttes... Mimir returnerer til normal drift.\n";
    }
    else if (input.rfind("gå ", 0) == 0) { //Sjekker om kommandoen starter med "gå"
        //Finner første mellomrom etter "gå " for å hente retningen
        size_t spacePos = input.find(' ');
        std::string direction = input.substr(spacePos + 1); //Henter alt etter mellomrommet

        if (currentRoom->exits.find(direction) != currentRoom->exits.end()) {
            currentRoom = currentRoom->exits[direction];
        } else {
            std::cout << "Du kan ikke gå den veien.\nTilgjengelige utveier er: ";
            for (const auto& exit : currentRoom->exits) {
                std::cout << "[" << exit.first << "] ";
            }
            std::cout << "\n";
        }
    }
    else if (input.rfind("ta ", 0) == 0) {
        size_t spacePos = input.find(' ');
        std::string itemName = input.substr(spacePos +1); //Henter alt etter mellomrommet

        auto it = std::find_if(currentRoom->items.begin(), currentRoom->items.end(), [&itemName](const item& item) {return item.name == itemName;});
    
        if (it != currentRoom->items.end()) {
            inventory.push_back(*it);
            std::cout << "Du plukket opp: " << it->name << "\n";
            currentRoom->items.erase(it); //Fjerner gjenstanden fra rommet
        } else {
            std::cout << "Den gjenstanden finnes ikke her.\n";
        }
    }
    else if (input == "inventory" || input == "inv" || input == "i" || input == "inventar" || input == "se sekk") {
        if (inventory.empty()) {
            std::cout << "Sekken din er tom.\n";
        } else {
            std::cout << "I sekken din har du:\n";
            for (const auto& item : inventory) {
                std::cout << " - " << item.name << " (" << item.description << ")\n";
            }
        }
    }
    else {
        std::cout << "Ukjent kommando. Prøv 'gå [retning]', 'ta [gjenstand]', 'inventory', eller 'avslutt'.\n";
    }
}
    return 0;
}
