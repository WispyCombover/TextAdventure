#include "RoomBlueprint.h"
#include <string>
#include <vector>

namespace {
    const std::vector<std::string> kSteps = { "bak", "old", "tmp", "final", "final2", "final_endelig" };

    const char* kJunctionText[] = {
        "Hyllene stiger til taket i begge retninger, fulle av mapper merket 'KUN FOR INTERNT BRUK'. Lysrørene flimrer i takt med noe du håper er en ventilasjon. Seks ganger fører videre: BAK, OLD, TMP, FINAL, FINAL2 og FINAL_ENDELIG.",
        "Gulvet er dekket av et tynt lag støv og en eller annen gammel pizzabit. Noen har festet en post-it på en hylle: 'Ikke bruk kopi (2).' Seks ganger fører videre: BAK, OLD, TMP, FINAL, FINAL2 og FINAL_ENDELIG.",
        "En vifte durer et sted i mørket, og luften lukter blekk og utgått byråkrati. På en reol står det med tusj: 'Under revisjon siden 1998'. Seks ganger fører videre: BAK, OLD, TMP, FINAL, FINAL2 og FINAL_ENDELIG.",
        "Her er hyllene sammenrast i en haug av mapper, og du må klatre over en stabel med arkivbokser merket 'SLETT 2011'. Seks ganger fører videre: BAK, OLD, TMP, FINAL, FINAL2 og FINAL_ENDELIG.",
        "Papirene ligger nå som snø i gangen, og et sted foran deg hører du noe stort puste sakte. Det rasler i arkene. Seks ganger fører videre: BAK, OLD, TMP, FINAL, FINAL2 og FINAL_ENDELIG."
    };

    // Seks utganger i hvert rom: bare steps[correct] går videre, resten sender deg tilbake til porten.
    void addAisles(RoomBlueprint& r, size_t correct, const std::string& nextId) {
        for (size_t i = 0; i < kSteps.size(); ++i) {
            const std::string target = (i == correct) ? nextId : std::string("archive_gate");
            r.exits.push_back({ kSteps[i], target, "", "En smal gang mellom to hyller. Etiketten er håndskrevet og delvis utvisket." });
        }
    }
}

const std::vector<RoomBlueprint>& getFloor_2Rooms() {
    static const std::vector<RoomBlueprint> rooms = [] {
        std::vector<RoomBlueprint> v;

        // Inngangen = steg 0 (riktig utgang: "bak")
        RoomBlueprint gate;
        gate.id = "archive_gate";
        gate.name = "Papirarkivet - inngangen";
        gate.description = "Trappen ender i et betongrom med lav himling og en dør som står permanent åpen. Foran deg strekker et arkiv seg inn i mørket: rekke på rekke av grå stålhyller, fylt med mapper ingen har åpnet siden forrige revisjon. På veggen henger et skilt: 'ARKIVET – kun for autorisert personell. Alle henvendelser må leveres skriftlig i tre eksemplarer.' Seks ganger fører videre: BAK, OLD, TMP, FINAL, FINAL2 og FINAL_ENDELIG. Trappen går tilbake OPP.";
        gate.discoveryXP = 10;
        addAisles(gate, 0, "archive_1");
        gate.exits.push_back({ "opp", "stairwell_f1", "", "Trappen tilbake opp til første etasje." });
        gate.scenery.push_back({ {"skilt"}, "Skiltet er laminert og har en uleselig underskrift. Under er det klistret en gul lapp: 'Takk for din tålmodighet.'" });
        gate.scenery.push_back({ {"pult", "praktikant"},
            "Bak en pult sitter det som en gang var en praktikant. Han ser ut til å ha ventet på svar fra HR siden 2009. Ved albuen hans står en militærgrønn termos.",
            "veteranens nødtermos" });
        v.push_back(gate);

        // Gang 1-5 (steg 1-5)
        for (size_t k = 1; k <= 5; ++k) {
            RoomBlueprint r;
            r.id = "archive_" + std::to_string(k);
            r.name = "Papirarkivet - gang " + std::to_string(k);
            r.description = kJunctionText[k - 1];
            r.discoveryXP = 3;
            const std::string next = (k < 5) ? "archive_" + std::to_string(k + 1) : std::string("archive_heart");
            addAisles(r, k, next);
            v.push_back(r);
        }

        // Arkivets hjerte (boss)
        RoomBlueprint heart;
        heart.id = "archive_heart";
        heart.name = "Papirarkivet - hjertet";
        heart.description = "Gangene åpner seg til en stor sirkulær hall der tusenvis av ark flyter i luften som støv i sollys. I midten ligger en tom reol der det tydeligvis har stått én enkelt bok. Ut-døren leder tilbake til inngangen.";
        heart.discoveryXP = 15;
        heart.fixedMonsterLevel = 3;
        heart.fixedMonsterName = "The Retention Policy";
        heart.exits.push_back({ "ut", "archive_gate", "", "Gangen leder tilbake til arkivets inngang." });
        heart.scenery.push_back({ {"reol", "hylle"}, "En tom hylle midt i rommet. Et merke i støvet viser at en bok med nøyaktig samme størrelse som et eksemplar av The Mythical Man-Month har stått her." });
        v.push_back(heart);

        return v;
    }();
    return rooms;
}