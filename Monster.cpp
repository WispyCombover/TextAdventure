#include "Monster.h"

monsterManual::monsterManual() {
    // Starter RNG (Mersenne Twister)
    std::random_device rd;
    rng = std::mt19937(rd());

    // MONSTEROVERSIKT - "blueprints"
    // LEVEL 1
    blueprintDatabase[1].push_back({
        "Merge Conflict Wizard",
        "En skikkelse kledd i hettegenseren til servicedesk. Den mumler uforståelige git-kommandoer.",
        1, 10, 10, 4, 5, 10,
        "Kaster en 'Git Push Force' som korrumperer filene dine!"
    });
    blueprintDatabase[1].push_back({
        "The Cookie Banner",
        "En vegg av uleselig juridisk tekst som blokkerer utsikten din. Den nekter å forsvinne.",
        1, 8, 8, 2, 7, 12,
        "Spammer deg med 'Godta alle' -knapper til du mister oversikten!"
    });
    blueprintDatabase[1].push_back({
        "The Missing Semicolon",
        "En mikroskopisk skapning som gjemmer seg i skyggene. Umulig å se med det blotte øyet.",
        1, 5, 5, 4, 10, 14,
        "Angriper i mørket, får deg til å snuble og kaster en 'Compilation Error' rett i ansiktet ditt!"
    });

    // LEVEL 2
    blueprintDatabase[2].push_back({
        "The Daily Standup Spectre",
        "Et gjenferd som tvinger deg til å stå oppreist mens det spør: 'Hva gjorde du i går?'.",
        2, 20, 20, 5, 15, 14,
        "Holder en endeløs monolog og påfører deg 'Feature Creep' -skade over tid!"
    });
    blueprintDatabase[2].push_back({
        "The Jira Ticket Overlord",
        "Et monster laget av post-it-lapper og uleste varlser. Den flytter deg konstant mellom 'In Progress' og 'To Do'.",
        2, 25, 25, 4, 15, 15,
        "Oppretter tre nye sub-tasks som du MÅ løse med én gang!"
    });

    // LEVEL 3
    blueprintDatabase[3].push_back({
        "The DNS Daemon",
        "Ingen vet hva det er, eller hvor det kommer fra, men alt slutter å fungere når det våkner.",
        3, 40, 40, 8, 22, 16,
        "Endrer IP-adressen din i skjul, så angrepene dine treffer deg selv i stedet!"
    });
    blueprintDatabase[3].push_back({
        "The COBOL Leviathan",
        "Et eldgammelt monster dekket av hullkort og støv. Ingen nålevende personer forstår hva det er.",
        3, 50, 50, 6, 35, 17,
        "Brøler et uforståelig feilsøkings-dump fra 1983 og lammer deg med nostalgi!"
    });

    // LEVEL 4
    blueprintDatabase[4].push_back({
        "Bobby Tables (The Dropper of Databases)",
        "En beryktet skikkelse som bærer på et skilt hvor det står: '; DROP TABLE Monsters;--'.",
        4, 75, 75, 12, 50, 18,
        "Kaster en SQL-injection som sletter vitale deler av dine essensielle funksjoner!"
    });

    // LEVEL 5
    blueprintDatabase[5].push_back({
        "The Deadline Monster",
        "En kolossal, tikkende skygge som blir større og rødere for hvert sekund som går. Den har en hær av uleste e-poster og kaffekopper rundt seg.",
        5, 200, 200, 25, 225, 19,
        "Kaster 'ASAP-panikk' over deg, spiser opp nattesøvnen din, og tvinger deg til å rulle ut uferdig kode i produksjon!"
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