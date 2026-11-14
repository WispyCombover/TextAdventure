#include "Item.h"
#include "Utils.h"
#include <iostream>

namespace {
    const std::vector<item> itemDatabase = {
        // ====== VÅPEN ======
        {
            .name = "kaffekopp",
            .description = "En mye brukt kaffekopp som noen har glemt igjen. Innsiden er misfarget av gammel kaffe. På utsiden står teksten 'There are 10 types of people in the world. Those who can read binary, and those who cannot.'",
            .value = 2,
            .currency = "kB",
            .type = "weapon",
            .damageDiceSides = 2,
            .damageDiceCount = 1,
            .damageType = "Bludgeoning",
            .extraDiceSides = 4,
            .extraDiceCount = 1,
            .extraDamageType = "acid",
            .isFilled = false,
            .throwable = true
        },
        {
            .name = "penn",
            .description = "En vanlig BIC-penn som har tørket inn. Improvisert stikkvåpen (1d4 skade).",
            .value = 1,
            .currency = "MB",
            .type = "weapon",
            .damageDiceSides = 4,
            .damageDiceCount = 1,
            .damageType = "Piercing",
            .throwable = false
        },
        {
            .name = "mekanisk tastatur",
            .description = "Et blytungt tastatur med Cherry MX Blue-brytere. Skaden er både fysisk og psykisk.",
            .value = 15,
            .currency = "MB",
            .type = "weapon",
            .damageDiceSides = 8,
            .damageDiceCount = 1,
            .damageType = "Bludgeoning",
            .extraDiceSides = 4,
            .extraDiceCount = 1,
            .extraDamageType = "sonic"
        },

        // ====== CONSUMABLES ======
        {
            .name = "rød monster",
            .description = "En lunken boks med sukker of koffein. Gir deg ny giv.",
            .value = 75,
            .currency = "kB",
            .type = "consumable",
            .effect = itemEffect::healHp,
            .effectDiceSides = 8,
            .effectDiceCount = 1,
            .effectFlatBonus = 2,
            .consumeText = ""
        },
        {
            .name = "grønn monster",
            .description = "Originalen. Lyser svakt radioaktivt i mørket. Gjenoppretter 2d4 +2 HP.",
            .value = 40,
            .currency = "kB",
            .type = "consumable",
            .effect = itemEffect::healHp,
            .effectDiceSides = 4,
            .effectDiceCount = 2,
            .effectFlatBonus = 2,
            .consumeText = "Du spretter boksen. *PSST!* Den syntetiske smaken av batterisyre og eple sender nytt liv gjennom årene dine."
        },
        {
            .name = "hvit monster",
            .description = "Monster Ultra. Sukkerfri og utviklernes foretrukne drivstoff. Gjenoppretter 2d6 Mana.",
            .value = 50,
            .currency = "kB",
            .type = "consumable",
            .effect = itemEffect::restoreMana,
            .effectDiceSides = 6,
            .effectDiceCount = 2,
            .effectFlatBonus = 0,
            .consumeText = "Du tar en slurk iskald hvit Monster. Hjernetåken letter og kodestrukturen blir krystallklar!"
        },
        {
            .name = "mango loco",
            .description = "En tropisk eksplosjon av sukker og koffein. Gir deg både HP (1d6) og en midlertidig koffein-buff!",
            .value = 75,
            .currency = "kB",
            .type = "consumable",
            .effect = itemEffect::caffeineRush,
            .effectDiceSides = 6,
            .effectDiceCount = 1,
            .effectFlatBonus = 2,
            .consumeText = "Du styrter en Mango Loco. Pulsen skyter i været, øynene dirrer og du føler deg ustoppelig!"
        },
        {
            .name = "gammel smultring",
            .description = "Lå igjen etter fredagskosen for to uker siden. Glasur med tvilsom farge.",
            .value = 5,
            .currency = "kB",
            .type = "consumable",
            .effect = itemEffect::sugarCrash,
            .effectDiceSides = 4,
            .effectDiceCount = 1,
            .effectFlatBonus = 0,
            .consumeText = "Du tar en bit. Den knaser på feil steder. Magen din angrer umiddelbart."
        },

        // ====== QUEST / MAGIC ======
        {
            .name = "necronomicon manualis",
            .description = "En tykk, skinnbundet bok med knekt rygg og kaffeflekker på hvert eneste oppslag. På forsiden står det innprentet med falmet gullskrift: «UNIX Programmer’s Manual, Bell Labs, 1971». Sidene er fylt med små, tette bokstaver og merkelige symboler, og margene er full av håndskrevne notater fra utviklere som for lengst har sluttet i firmaet. Boken åpner seg alltid på siden du ikke trenger. Det sies at ingen noensinne har lest den ferdig, og at de som påstår det lyver.",
            .value = 0,
            .currency = "kB",
            .type = "spellbook",
            .isQuestItem = true
        },
        {
            .name = "adgangskort",
            .description = "Et hvitt plastkort med et falmet bilde av en utvikler som så mye gladere ut på ansettelsesdagen.",
            .value = 0,
            .currency = "kB",
            .type = "key",
            .isQuestItem = true
        }
    };
}  // namespace

const std::vector<item>& itemList() {
    return itemDatabase;
}

bool itemExists(const std::string& name) {
    std::string wanted = toLowerNorwegian(name);
    for (const auto& it : itemDatabase) {
        if (toLowerNorwegian(it.name) == wanted) {
            return true;
        }
    }
    return false;
}

item getItem(const std::string& name) {
    std::string wanted = toLowerNorwegian(name);
    for (const auto& it : itemDatabase) {
        if (toLowerNorwegian(it.name) == wanted) {
            return it;
        }
    }

    std::cerr << "[DEBUG] Advarsel: Fant ikke item '" << name << "' i itemDatabase!\n";
    return item{
        .name = "undefined (" + name + ")",
        .description = "En merkelig glitch i virkeligheten. Noen har stavet et item-navn feil i koden.",
        .value = 0,
        .currency = "kB",
        .type = "junk"
    };
}

void inspectItem(const item& it) {
    std::cout << "\n=== " << toUpperNorwegian(it.name) << " ===\n";
    std::cout << it.description << "\n";

    //Stats basert på item-type
    if (it.type == "consumable") {
        std::cout << "\n[Næringsinnhold & Bivirkninger]\n";

        //Terninger
        std::string formula = std::to_string(it.effectDiceCount) + "d" + std::to_string(it.effectDiceSides);
        if (it.effectFlatBonus > 0) {
            formula += " + " + std::to_string(it.effectFlatBonus);
        }

        switch (it.effect) {
            case itemEffect::healHp:
                std::cout << "  * Helse-boost:    +" << formula << " HP\n";
                std::cout << "  * Koffein:        160mg (Godkjent av arbeidsmiljøloven)\n";
                break;

            case itemEffect::restoreMana:
                std::cout << "  * Fokus-boost:    +" << formula << " Mana\n";
                std::cout << "  * Sukker:         0 g (Ren utvikler-magi)\n";
                break;
            
            case itemEffect::caffeineRush:
                std::cout << "  * Energi-sjokk:   +" << formula << " HP + [Koffeinsjokk] (3 runder)\n";
                std::cout << "  * Advarsel:       Kan føre til ukontrollert refaktorering av produksjonskode.\n";
                break;

            case itemEffect::sugarCrash:
                std::cout << "  * Risiko:         -" << formula << " HP (Matforgiftning)\n";
                std::cout << "  * Utløpsdato:     Uleselig (trolig fra forrige kvartal)\n";
                break;

            default:
                std::cout << "  * Effekt:         Ukjent kjemisk sammensetning\n";
                break;
        }
    }
    else if (it.type == "weapon") {
        std::cout << "\n[Spesifikasjoner]\n";
        std::cout << "  * Skade:         " << it.damageDiceCount << "d" << it.damageDiceSides << " (" << it.damageType << ")\n";

        if (it.extraDiceCount > 0) {
            std::cout << "  * Bonusskade:    " << it.extraDiceCount << "d" << it.extraDiceSides << " (" << it.extraDamageType << ")";
            if (it.name == "kaffekopp" && !it.isFilled) {
                std::cout << " [Krever varm kaffe]";
            }
            std::cout << "\n";
        }
        if (it.throwable) {
            std::cout << "  * Egenskap:     Kastbar (engangs i kamp)\n";
        }
    }
    // Verdi og quest-status nederst
    if (it.isQuestItem) {
        std::cout << "  * Status:    Viktig gjenstand (kan ikke kastes/selges)\n";
    } else if (it.value > 0) {
        std::cout << "  * Markedsverdi:  " << it.value << " " << it.currency << "\n";
    }
    std::cout << "=========================\n";
}