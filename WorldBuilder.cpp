#include "WorldBuilder.h"
#include "Item.h"
#include "Monster.h"
#include "NPC.h"

Room* buildWorld(std::vector<std::unique_ptr<Room>>& allRooms) {
    //BYGG ROMMENE HER
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

    //UTGANGER HER
    spawnRoom->exits["dør"] = corridor.get();
    corridor->exits["dør"] = spawnRoom.get();
    corridor->exits["vest"] = corridorWest.get();
    corridor->exits["øst"] = corridorEast.get();
    corridorWest->exits["øst"] = corridor.get();
    corridorEast->exits["vest"] = corridor.get();

    // LEGG TIL ITEMS
    spawnRoom->items.push_back(item{"kaffekopp", "En mye brukt kaffekopp som noen har glemt igjen. Innsiden er misfarget av gammel kaffe. Verdi: ", 2, "kB", "weapon", 2, 1, "Bludgeoning", 0, 0, "acid", false, true});
    spawnRoom->items.push_back(item{"penn", "En vanlig BIC-penn som har tørket inn. Improvisert stikkvåpen (1d4 skade). Verdi: ", 1, "MB", "weapon", 4, 1, "Piercing", 0, 0, "", false, false});

    // RANDOM ENCOUNTERS
    monsterManual manual;
    corridorEast->monster = manual.getRandomMonster(1);  // Spawner et tilfeldig level 1 monster i østkorridoren

    // NPCer
    //openOfficeOne->npcs.push_back(createFirewallGuru());

    //PEKER TIL SPAWN
    Room* startingRoomPtr = spawnRoom.get();

    //LEGG ROMMENE I allRooms-vektoren
    allRooms.push_back(std::move(spawnRoom));
    allRooms.push_back(std::move(corridor));
    allRooms.push_back(std::move(corridorWest));
    allRooms.push_back(std::move(corridorEast));

    //SEND SPAWN TIL game.cpp
    return startingRoomPtr;
    
}