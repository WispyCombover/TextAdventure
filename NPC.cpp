#include "NPC.h"
#include "Item.h"
#include "Utils.h"

std::unique_ptr<NPC> createFirewallGuru(const player& p) {
    auto guru = std::make_unique<NPC>();
    guru->name = "Brannmur-Guruen";
    guru->description = "En småtrøtt seniorutvikler med tre skjermer og en tom boks hvit Monster™.";
    guru->isMerchant = true;

    std::string playerName = p.name.empty() ? "Utvikler" : p.name;

    // Node 0: Start på dialog
    guru->dialogueTree.push_back({
        "Hvem der? Jeg holder på å patche en zero-day i kaffetrakteren. Hva vil du?",
        {
            {"Jeg er " + playerName + ". Jeg jakter på Deadlinemonsteret™. Har du sett det?", 1, false},
            {"Har du noe utstyr til salgs?", 2, true},
            {"Beklager forstyrrelsen. (Avslutt samtalen)", -1, false}
        }
    });

    // Node 1: Montserinfo
    guru->dialogueTree.push_back({
        "Det beistet stjal produksjonsnøklene og løp mot øst! Men du trenger bedre gear enn det der for å bekjempe noe sånt, unge eventyrer!",
        {
            {"Hva har du å selge?", 2, true},
            {"Takk for tipset! (Avslutt samtalen)", -1, false}
        }
    });

    // Node 2: Butikk
    guru->dialogueTree.push_back({
        "Ta en titt på det jeg har liggende. Alt betales i ren båndbredde!",
        {
            {"(Avslutt samtale)", -1, false}
        }
    });

    //Legg til varer i butikken her
    guru->shopInventory.push_back(getItem("mekanisk tastatur"));
    guru->shopInventory.push_back(getItem("rød monster"));

    return guru;
}