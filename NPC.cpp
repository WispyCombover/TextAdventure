#include "NPC.h"
#include "Item.h"
#include "Utils.h"

std::unique_ptr<NPC> createSlitenSenior(const player& p) {
    auto senior = std::make_unique<NPC>();
    senior->name = "Gretten Seniorutvikler";
    senior->description ="En utslitt seniorutvikler med mørke ringer under øynene, bustete hår og en falmet hettegenser. Han stirrer oppgitt på deg mens han tviholder på et halvt oppspist knekkebrød med brunost.";
    senior->isMerchant = false;

    std::string playerName = p.name.empty() ? "junior" : p.name;

    // Node 0: Start på diaolg
    senior->dialogueTree.push_back({
        "Hallooo?! Så du ikke lappen på døren?! Jeg var ETT sekund unna å forklare Anda hvorfor den pekeren ble null etter refaktoreringen! Hva vil du, " + playerName + "?",
        {
            {"Beklager! Jeg prøver å komme meg inn på rekvisitarommet. Vet du koden?", 1, false},
            {"Hva er greia med den store gummianda på bordet?", 2, false},
            {"Unnskyld forstyrrelsen, jeg skal gå nå. (Avslutt samtalen)", -1, false}
        }
    });

    // Node 1: Hint til koden til rekvisita (1357)
    senior->dialogueTree.push_back({
        "*Sukk*... Hør her. Ledelsen bytter aldri passord, men de tror de er smarte. Koden til rekvisita er de fire første oddetallene – i strengt stigende rekkefølge. Klarer du å regne ut det selv, eller trenger du en stackoverflow-tråd for det også?",
        {
            {"Takk! Hva med den lille gummianda som ligger på bordet der?", 3, false},
            {"Skjønner. Lykke til med feilsøkingen! (Avslutt samtalen)", -1, false}
        }
    });

    // Node 2: Om anda
    senior->dialogueTree.push_back({
        "Det er Sjefsarkitekten. Hun sier ingenting, dømmer deg ikke når koden din ser ut som spagetti, og finner flere bugs enn hele QA-avdelingen til sammen. Vis litt respekt.",
        {
            {"Fair enough. Du, vet du tilfeldigvis koden til rekvisitarommet?", 1, false},
            {"Jeg lar dere jobbe i fred. (Avslutt samtalen)", -1, false}
        }
    });

    // Node 3: Den lille anda
    senior->dialogueTree.push_back({
        "Den lille? Ta den hvis du vil. Det er en reise-and. Kjekk å ha i lomma hvis du møter på uforklarlige feil ute i kontorlandskapet. Bare lukk døren etter deg når du går!",
        {
            {"Takk for hjelpen! (Avslutt samtalen)", -1, false}
        }
    });

    return senior;    
}
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