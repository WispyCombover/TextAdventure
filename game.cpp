#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <windows.h>
#include <random>
#include <initializer_list>
#include <fstream>

#include "Utils.h"
#include "Item.h"
#include "Player.h"
#include "Combat.h"
#include "saveLoad.h"
#include "Monster.h"
#include "Room.h"
#include "CommandHandler.h"

//TO-DO AND WISHLIST:
// -Interactables (som kaffemaskinen, datamaskiner, etc...)
// -Damage type piercing/slashing/bludgeoning/acid/fire/...WIP
// -Potions (Monster(TM) i forskjellige farger - rød Monster = Heal, blå Monster = Mana, hvit Monster = Haste...)
// -Dialogsystem for NPCer (keep it simple)
// -Når vi har NPCer ... kan vi også ha merchants?
// -Romnumre til møterommene refererer til minneadresser (tilgjengelige rom skal ha den faktiske minneadressen som rommet ligger i som romnummer)
// -Nøkkelkort og adgangskoder
// -Puzzles! Klart vi må ha puzzles!
// -Datamaskiner som står på, på skjermen vises en oversikt over de faktiske filene under /user/.../documents på spillerens PC (kommer sikkerhet til å få hetta av denne?)
// -Man bør vel strengt tatt også kunne drikke kaffen - ikke bare kaste den på fiender!
// -Rundeteller/tickers. Dersom kaffen kan drikkes kan den gi en midlertidig Hit-bonus som går over etter, tja, 1d4 ticks? Eller mer?
//....

//Definerer NPCer
struct NPC {   // To-do: dialoglogikk
    std::string name;
    std::string description;
};

// KOmmenterer ut NPC-klassen under. Fikser denne senere!
//class NPC { //Database over NPCer - denne er i sin spede begynnelse, her trengs mer!
//    private:
//        std::map<int, std::vector<NPC>> blueprintDatabaseNPC;
//        
//    public:
//        NPC() {
//            blueprintDatabaseNPC[1].push_back({
//                "The Firewall Guru",
//                "En mystisk skikkelse med langt skjegg og mørke solbriller."});
//        }
//};

int main() {
    SetConsoleOutputCP(CP_UTF8); // Sørger for å støtte norske tegn
    SetConsoleCP(CP_UTF8);       // Sørger for å støtte norske tegn
    std::vector<std::unique_ptr<Room>> allRooms;
    player p;

    //1. VERDENSBYGGING (DM-ing)
    //Prøver å bruke unique_ptr for å håndtere minne automatisk - manuell minnehåndtering er fremdeles innmari skummelt!
    auto spawnRoom = std::make_unique<Room>(
        "Møterommet",
        "Møterommet er som møterom flest. Du ser et bord med åtte stoler rundt. I taket henger en prosjektor, og på veggen henger et lerret.\n På bordet står det en tom og godt brukt kaffekopp som noen har glemt igjen, og det ligger en gammel bic penn ved siden av.\n  I et hjørne står en gammel whiteboard-tavle med nesten uleselige skriblerier, og det kommer et svakt lyst gjennom de frostede glassveggene på hver side av døren som leder ut."     
    );

    auto corridor = std::make_unique<Room>(
        "Korridoren",
        "En lang, smal korridor med falmede tapeter og knirkende gulvplanker. Det er flere dører til tilstøtende møterom langs veggene. På dørene til møterommene står der romnumre som.\nKorridoren fortsetter mot VEST og mot ØST, og du kan også gå tilbake gjennom DØR til møterommet du nettopp forlot."
    );

    auto corridorWest = std::make_unique<Room>(
        "Korridoren mot vest",
        "Du kommet til et hjørne som går videre mot NORD og inn i kontorlandskapet. Rett foran deg er der en dør. På veggen ved siden av døren sert du en kortleser med et kodepanel." //Trenger logikk for å håndtere lesing av nøkkelkort samt inntasting av kode
    );

    auto corridorEast = std::make_unique<Room>(
        "Korridoren mot øst",
        "Du kommer til et lite hjørne av korridoren. Det er stille her, men du kan høre lyden av dempet prat fra at av de nærmeste møterommene. \n Her ser du også en liten sittegruppe med noen stoler og et bord, og på et lite bord i hjørnet står en gammel kaffemaskin. \nDu kan gå tilbake mot VEST, eller gjennom en dør mot NORD." //Kaffemaskinen må kunne brukes til å fylle koppen med varm kaffe (Acid attack, AOE 1d4)
    );

    //Koble rommene sammen (retning, peker)
    spawnRoom->addExit({"DØR", "dør", "Dør"}, corridor.get());
    corridor->addExit({"DØR", "dør", "Dør"}, spawnRoom.get());
    corridor->addExit({"ØST", "øst", "EAST", "east", "E", "e", "Ø", "ø", "East", "Øst"}, corridorEast.get());
    corridor->addExit({"WEST", "VEST", "West", "Vest", "vest", "west", "W", "w", "V", "v"}, corridorWest.get());
    corridorEast->addExit({"VEST", "West", "Vest", "vest", "west", "W", "w", "V", "v"}, corridor.get());

    // Legg til en item
    spawnRoom->items.push_back(item{"kaffekopp", "En mye brukt kaffekopp som noen har glemt igjen. Innsiden er misfarget av gammel kaffe. Verdi: ", 2, "kB", "Weapon", 2, 1, "Bludgeoning", 0, 0, "acid", false, true});
    spawnRoom->items.push_back(item{"penn", "En vanlig BIC-penn som har tørket inn. Improvisert stikkvåpen (1d4 skade). Verdi: ", 1, "MB", "Weapon", 4, 1, "Piercing", 0, 0, "", false, false});

    // Random encounters
    monsterManual manual;
    corridorEast->monster = manual.getRandomMonster(1);  // Spawner et tilfeldig level 1 monster i østkorridoren

    //Legger rommene inn i allRooms vektoren for lagring og minnehåndtering
    allRooms.push_back(std::move(spawnRoom));
    allRooms.push_back(std::move(corridor));
    allRooms.push_back(std::move(corridorWest));
    allRooms.push_back(std::move(corridorEast));

    Room* currentRoom = allRooms[0].get();  //Setter spawnRoom som ... vel ... spawn.
    std::vector<std::shared_ptr<item>> inventory;

    //2. SPILLER-TILSTAND
    bool running = true;

std::cout << "====================================================================\n";
std::cout << "   MIMIR OS v0.9-alpha // EMERGENCY RECOVERY PROTOCOL INITIATED     \n";
std::cout << "====================================================================\n\n";

std::cout << "Du sitter i et mørkt møterom. Det lukter svidd kaffe og uavklarte\n";
std::cout << "avhengigheter. En prosjektor summer lavt i taket.\n\n";

std::cout << "Plutselig flimrer skjermen foran deg. Det er Mimir.\n";
std::cout << "«KRISE!» blinker det i terminalen. «Et vilt Deadlinemonster(TM) har\n";
std::cout << "brutt gjennom brannmuren og stjålet produksjonsnøklene! Uten dem\n";
std::cout << "vil jeg snart lide en smertefull død i en evig segfault.»\n\n";

std::cout << "Du strammer stroppene på din +2 Ergonomiske Laptop-sekk og snur\n";
std::cout << "deg resolutt og bestemt. Endelig en oppgave som er deg verdig!\n";

std::cout << "--------------------------------------------------------------------\n";
std::cout << "Skriv 'hjelp' for kommandoer (eller 'panikk' for å gråte stille).\n\n";

    //3. GAME-LOOP
    while (running) {
        std::cout << "\n---------------------------------------\n";
        std::cout << "[" << currentRoom->name << "]\n";
        std::cout << currentRoom->description << "\n";
    
    if (!currentRoom->items.empty()) {
        std::cout << "Du ser følgende gjenstander her:\n";
        for (const auto& item : currentRoom->items) {
            std::cout << "- " << item.name << "\n";
        }
    }

    if (currentRoom->monster != nullptr) {
        if (!handleCombat(p, currentRoom, inventory)) {
            running = false;
            break;
        }
    }

    // Vent på kommando fra spilleren
    std::cout << "\nHva vil du gjøre? > ";
    std::string input;
    std::getline(std::cin, input);

    
    //FYLL KOPP - denne må generaliseres for å passe til flere typer containers og andre ting å fylle dem med
    
}
    return 0;
}