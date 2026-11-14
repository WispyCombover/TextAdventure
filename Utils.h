#pragma once
#include <string>
#include <utility>

int rollDice(int sides);

long long convertToKB(long long amount, const std::string& unit);
std::string formatDataCurrency(long long kb);

inline std::string toLowerNorwegian(std::string text) {
    //Standard ASCII A-Z
    for (char& c : text) {
        if (c >= 'A' && c <= 'Z') {
            c = static_cast<char>(c + ('a' - 'A'));
        }
    }
    //Erstatter ÆØÅ med æøå
    const std::pair<std::string, std::string> norwegianChars[] = {
        {"Æ", "æ"},
        {"Ø", "ø"},
        {"Å", "å"}
    };

    for (const auto& [upper, lower] : norwegianChars) {
        size_t pos = 0;
        while ((pos = text.find(upper, pos)) != std::string::npos) {
            text.replace(pos, upper.length(), lower);
            pos += lower.length();
        }
    }
    return text;
}

inline std::string toUpperNorwegian(std::string text) {
    //Standard ASCII A-Z
    for (char& c : text) {
        if (c >= 'A' && c <= 'Z') {
            c = static_cast<char>(c + ('a' - 'A'));
        }
    }
    //Erstatter ÆØÅ med æøå
    const std::pair<std::string, std::string> norwegianChars[] = {
        {"æ", "Æ"},
        {"ø", "Ø"},
        {"å", "Å"}
    };

    for (const auto& [lower, upper] : norwegianChars) {
        size_t pos = 0;
        while ((pos = text.find(lower, pos)) != std::string::npos) {
            text.replace(pos, lower.length(), lower);
            pos += upper.length();
        }
    }
    return text;
}