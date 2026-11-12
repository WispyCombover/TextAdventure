#include "Utils.h"
#include <random>
#include <iomanip>
#include <sstream>

long long convrtToKB(long long amount, const std::string& unit) {
    std::string u = toLowerNorwegian(unit);
    if (u == "mb") return amount * 1024LL;
    if (u == "gb") return amount * 1024LL * 1024LL;
    if (u == "tb") return amount * 1024LL * 1024LL * 1024LL;
    return amount;  //Standard settes til kb for enkelthets skyld
}

std::string formatDataCurrency(long long kb) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);

    const long long MB = 1024LL;
    const long long GB = MB * 1024LL;
    const long long TB = GB * 1024LL;

    if (kv >= TB) {
        oss << (static_cast<double>(kb) / TB) << " TB";
    } else if (kb >= GB) {
        oss << (static_cast<double>(kb) / GB) << " GB";
    } else if (kb >= MB) {
        oss << (static_cast<double>(kb) / MB) << " MB";
    } else {
        return oss.str();
    }
}

int rollDice(int sides) {
    if (sides <= 0) return 0;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, sides);
    return dis(gen);
}