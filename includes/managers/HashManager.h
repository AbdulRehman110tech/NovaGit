// includes/managers/HashManager.h
#pragma once

#include <cstddef>
#include <string>

class HashManager {
public:
    HashManager();

    std::string generateHash(
        const std::string& data
    ) const;

private:
    std::string toHexString(
        std::size_t value
    ) const;
};
