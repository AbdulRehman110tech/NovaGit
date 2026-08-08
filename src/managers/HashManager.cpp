// src/managers/HashManager.cpp
#include "managers/HashManager.h"

namespace {

constexpr std::size_t kHashBase = 131;
constexpr std::size_t kHashModulus = 1000000007;

} // namespace

HashManager::HashManager() {
}

std::string HashManager::generateHash(
    const std::string& data
) const {
    std::size_t value = 0;

    for (const char byte : data) {
        value = (value * kHashBase + static_cast<unsigned char>(byte)) % kHashModulus;
    }

    return toHexString(value);
}

std::string HashManager::toHexString(
    std::size_t value
) const {
    if (value == 0) {
        return "0";
    }

    std::string hex;

    while (value > 0) {
        const std::size_t digit = value % 16;
        const char hexDigit = digit < 10
            ? static_cast<char>('0' + digit)
            : static_cast<char>('A' + digit - 10);
        hex.insert(hex.begin(), hexDigit);
        value /= 16;
    }

    return hex;
}
