// src/managers/HeadManager.cpp
#include <filesystem>
#include <fstream>
#include <string>

#include "managers/HeadManager.h"

namespace fs = std::filesystem;

namespace {

constexpr char kHeadFilePath[] = ".novagit/HEAD";

} // namespace

HeadManager::HeadManager() {
}

std::string HeadManager::getCurrentCommitID() const {
    if (!isInitialized()) {
        return std::string{};
    }

    std::ifstream in(kHeadFilePath);

    if (!in) {
        return std::string{};
    }

    std::string commitID;
    std::getline(in, commitID);

    return commitID;
}

bool HeadManager::updateHEAD(
    const std::string& commitID
) {
    std::ofstream out(kHeadFilePath, std::ios::out | std::ios::trunc);

    if (!out) {
        return false;
    }

    out << commitID;

    return out.good();
}

bool HeadManager::isInitialized() const {
    return fs::exists(kHeadFilePath);
}
