#include "Monster.h"
#include "Item.h"

monsterManual::monsterManual() {
    // Starter RNG (Mersenne Twister)
    std::random_device rd;
    rng = std::mt19937(rd());

    // MONSTEROVERSIKT - "blueprints"
    // LEVEL 1
    blueprintDatabase[1].push_back({
        .name = "Merge Conflict Wizard",
        .description = "En skikkelse kledd i hettegenseren til servicedesk. Den mumler uforståelige git-kommandoer.",
        .level = 1, 
        .hp = 10, 
        .maxHp = 10, 
        .attackDamage = 4, 
        .xpReward = 5, 
        .monsterAC = 10,
        .attackMessage = "Kaster en 'Git Push Force' som korrumperer filene dine!",
        .minDataReward = 128,
        .maxDataReward = 512,
        .lootTable = {
            { getItem("rød monster"), 75 },
            { getItem("gammel smultring"), 5 }
        }
    });
    blueprintDatabase[1].push_back({
        .name = "The Cookie Banner",
        .description = "En vegg av uleselig juridisk tekst som blokkerer utsikten din. Den nekter å forsvinne.",
        .level = 1, 
        .hp = 8, 
        .maxHp = 8, 
        .attackDamage = 2, 
        .xpReward = 7, 
        .monsterAC = 12,
        .attackMessage = "Spammer deg med 'Godta alle' -knapper til du mister oversikten!"
    });
    blueprintDatabase[1].push_back({
        .name = "The Missing Semicolon",
        .description = "En mikroskopisk skapning som gjemmer seg i skyggene. Umulig å se med det blotte øyet.",
        .level = 1, 
        .hp = 5, 
        .maxHp = 5, 
        .attackDamage = 3, 
        .xpReward = 10, 
        .monsterAC = 14,
        .attackMessage = "Angriper i mørket, får deg til å snuble og kaster en 'Compilation Error' rett i ansiktet ditt!"
    });

    // LEVEL 2
    blueprintDatabase[2].push_back({
        .name = "The Daily Standup Spectre",
        .description = "Et gjenferd som tvinger deg til å stå oppreist mens det spør: 'Hva gjorde du i går?'.",
        .level = 2, 
        .hp = 20, 
        .maxHp = 20, 
        .attackDamage = 5, 
        .xpReward = 15, 
        .monsterAC = 14,
        .attackMessage = "Holder en endeløs monolog og påfører deg 'Feature Creep' -skade over tid!"
    });
    blueprintDatabase[2].push_back({
        .name = "The Jira Ticket Overlord",
        .description = "Et monster laget av post-it-lapper og uleste varlser. Den flytter deg konstant mellom 'In Progress' og 'To Do'.",
        .level = 2, 
        .hp = 25, 
        .maxHp = 25, 
        .attackDamage = 4, 
        .xpReward = 15, 
        .monsterAC = 15,
        .attackMessage = "Oppretter tre nye sub-tasks som du MÅ løse med én gang!"
    });

    // LEVEL 3
    blueprintDatabase[3].push_back({
        .name = "The DNS Daemon",
        .description = "Ingen vet hva det er, eller hvor det kommer fra, men alt slutter å fungere når det våkner.",
        .level = 3, 
        .hp = 40, 
        .maxHp = 40, 
        .attackDamage = 8, 
        .xpReward = 22, 
        .monsterAC = 16,
        .attackMessage = "Endrer IP-adressen din i skjul, så angrepene dine treffer deg selv i stedet!"
    });
    blueprintDatabase[3].push_back({
        .name = "The COBOL Leviathan",
        .description = "Et eldgammelt monster dekket av hullkort og støv. Ingen nålevende personer forstår hva det er.",
        .level = 3, 
        .hp = 50, 
        .maxHp = 50, 
        .attackDamage = 6, 
        .xpReward = 35, 
        .monsterAC = 17,
        .attackMessage = "Brøler et uforståelig feilsøkings-dump fra 1983 og lammer deg med nostalgi!"
    });

    // LEVEL 4
    blueprintDatabase[4].push_back({
        .name = "Bobby Tables (The Dropper of Databases)",
        .description = "En beryktet skikkelse som bærer på et skilt hvor det står: '; DROP TABLE Monsters;--'.",
        .level = 4, 
        .hp = 75, 
        .maxHp = 75, 
        .attackDamage = 12, 
        .xpReward = 50, 
        .monsterAC = 18,
        .attackMessage = "Kaster en SQL-injection som sletter vitale deler av dine essensielle funksjoner!"
    });

    // LEVEL 5
    blueprintDatabase[5].push_back({
        .name = "The Deadline Monster",
        .description = "En kolossal, tikkende skygge som blir større og rødere for hvert sekund som går. Den har en hær av uleste e-poster og kaffekopper rundt seg.",
        .level = 5, 
        .hp = 200, 
        .maxHp = 200, 
        .attackDamage = 25, 
        .xpReward = 225, 
        .monsterAC = 19,
        .attackMessage = "Kaster 'ASAP-panikk' over deg, spiser opp nattesøvnen din, og tvinger deg til å rulle ut uferdig kode i produksjon!"
    });
}

std::unique_ptr<monster> monsterManual::getMonsterBlueprint(int level, const std::string& name) {
    if (blueprintDatabase.find(level) != blueprintDatabase.end()) {
        for (const auto& m : blueprintDatabase[level]) {
            if (m.name == name) {
                return std::make_unique<monster>(m);
            }
        }
    }
    return nullptr;
}

std::unique_ptr<monster> monsterManual::getRandomMonster(int level) {
    if (blueprintDatabase.find(level) != blueprintDatabase.end() && !blueprintDatabase[level].empty()) {
        std::uniform_int_distribution<size_t> dist(0, blueprintDatabase[level].size() - 1);
        size_t randomIndex = dist(rng);
        return std::make_unique<monster>(blueprintDatabase[level][randomIndex]);
    }
    return nullptr;
}