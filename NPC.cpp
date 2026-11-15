#include "NPC.h"
#include "Item.h"
#include "Utils.h"

std::unique_ptr<NPC> createLockedInSecopsVoice(const player& p) {
    auto voice = std::make_unique<NPC>();
    voice->name = "Stemme fra kopirommet";
    voice->description = "Du kan ikke se hvem det er gjennom døren, men basert på den desperate bankinen er det noen som har vært sperret inne alt for lenge.";
    voice->isMerchant = false;

    std::string playerName = p.name.empty() ? "Du der ute" : p.name;

    //Node 0: første kontakt gjennom døren
    voice->dialogueTree.push_back({
        "*BANK! BANK! BANK!* Hallo?! Er det noen der ute?! For guds skyld, du må slippe meg ut! Døren smalt igjen da jeg gikk inn for å bytte toner, og denne døren kan bare åpnes fra utsiden! Trolig et brudd på flere HMS-forskrifter!",
        {
            {"Hvem er du, og hvordan havnet du der inne?", 1, false},
            {"Hvordan får jeg opp døren?", 2, false},
            {"Hold ut, jeg skal finne en vei inn! (Avslutt samtalen)", -1, false}
        }
    });

    //Node 1: Hvem og hvorfor (og hint til adgangskort)
    voice->dialogueTree.push_back({
        "Jeg jobber i SecOps! Jeg burde visst bedre enn noen, men jeg la igjen adgangskortet mitt på pulten da jeg løp for å hente utskrifter. Smekk – Zero-Trust-politikken slo inn! Nå har jeg sittet her med en skriver som piper 'PC LOAD LETTER' hvert tredje minutt. Jeg holder på å miste forstanden!",
        {
            {"Greit, hvordan får jeg låst opp døren?", 2, false},
            {"Jeg skal se hva jeg kan gjøre. (Avslutt samtalen)", -1, false}
        }
    });

    //Node 2: konklusjon...
    voice->dialogueTree.push_back({
        "Du må finne et gyldig adgangskort og bruke det på kortleseren ved siden av døren! Som sagt la jeg igjen mitt på pulten min i SecOps-avdelingen, ellers hørte jeg skrik og brak borte ved Sala Segfault tidligere i dag ... kanskje en av utviklerne mistet kortet sitt der inne? Vær så snill, " + playerName + ", finn et kort og slipp meg ut før skriveren vinner!",
        {
            {"Jeg går for å finne et adgangskort med én gang! (Avslutt samtalen)", -1, false}
        }
    });

    return voice;
}

std::unique_ptr<NPC> createFreedSecops(const player& p) {
    auto secops = std::make_unique<NPC>();
    secops->name = "Utslitt SecOps-analytiker";
    secops->description = "En blek sikkerhetsanalytiker i sort hettegenser med en halvdrukket termos og et lettet blikk. Hun ser ut som om hun nettopp har overlevd en gisselsituasjon med en HP LaserJet.";
    secops->isMerchant = false;

    std::string playerName = p.name.empty() ? "helt" : p.name;

    //Node 0: takk og belønning
    secops->dialogueTree.push_back({
        "Du klarte det! Endelig frisk luft ... vel, resirkulert kontorluft, men likevel! Tusen takk, " + playerName + "! Hadde jeg måttet høre på den skriveren ett minutt til, hadde jeg revet ut strømkabelen med tennene.",
        {
            {"Bare hyggelig! Hva gjorde du egentlig her inne?", 1, false, "laminert GDPR-skjold"},
            {"Vet du hva som foregår på kontoret i dag? Alt virker helt kaos.", 2, false},
            {"Godt å se deg i live. Jeg må videre! (Avslutt samtalen)", -1, false}
        }
    });

    //Node 1: Lore
    secops->dialogueTree.push_back({
        "Jeg skulle bare skrive ut en fysisk backup av brannmur-reglene. Ironisk nok ble jeg stengt inne av min egen tilgangskontroll. Forresten ... jeg fant et laminert GDPR-skjold bak makuleringsmaskinen mens jeg lette etter en nødutgang. Ta det, du trenger det nok mer enn meg!",
        {
            {"Hva skjer egentlig på resten av kontoret?", 2, false},
            {"Takk for skjoldet! Ta vare på deg selv. (Avslutt samtalen)", -1, false}
        }
    });

    //Node 2: Hint for videre gameplay
    secops->dialogueTree.push_back({
        "Hele systemet gikk i vranglås da Deadlinemonsteret™ brøt seg inn i produksjonsmiljøet. Hvis du skal stoppe det, bør du stikke innom Brannmur-Guruen i kontorlandskapet mot nordvest for å skaffe deg skikkelig utstyr først!",
        {
            {"Notert! Takk for hjelpen. (Avslutt samtalen)", -1, false}
        }
    });

    return secops;
}

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