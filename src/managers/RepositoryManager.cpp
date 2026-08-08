// src/managers/RepositoryManager.cpp
#include <filesystem>
#include <fstream>

#include "managers/RepositoryManager.h"

namespace fs = std::filesystem;

namespace {

constexpr char kRepositoryDirectory[] = ".novagit";
constexpr char kObjectsDirectory[] = ".novagit/objects";
constexpr char kCommitsDirectory[] = ".novagit/commits";
constexpr char kHeadFilePath[] = ".novagit/HEAD";
constexpr char kIndexFilePath[] = ".novagit/index";

} // namespace

RepositoryManager::RepositoryManager()
    : indexManager{},
      commitManager{},
      headManager{},
      index{} {
    if (repositoryExists()) {
        indexManager.loadIndexFromDisk(index);
    }
}

bool RepositoryManager::repositoryExists() const {
    return fs::exists(kRepositoryDirectory)
        && fs::exists(kObjectsDirectory)
        && fs::exists(kCommitsDirectory)
        && fs::exists(kHeadFilePath)
        && fs::exists(kIndexFilePath);
}

bool RepositoryManager::init() {
    if (repositoryExists()) {
        return false;
    }

    if (!fs::create_directory(kRepositoryDirectory)) {
        return false;
    }

    if (!fs::create_directory(kObjectsDirectory)) {
        return false;
    }

    if (!fs::create_directory(kCommitsDirectory)) {
        return false;
    }

    std::ofstream headFile(kHeadFilePath);

    if (!headFile) {
        return false;
    }

    std::ofstream indexFile(kIndexFilePath);

    if (!indexFile) {
        return false;
    }

    return true;
}

bool RepositoryManager::add(
    const std::string& filename,
    const std::string& content
) {
    if (!repositoryExists()) {
        return false;
    }

    return indexManager.stageFile(
        index,
        filename,
        content
    );
}

bool RepositoryManager::commit(
    const std::string& message
) {
    if (!repositoryExists()) {
        return false;
    }

    std::string parentCommitID = headManager.getCurrentCommitID();

    Commit commit = commitManager.createCommit(
        index,
        parentCommitID,
        message
    );

    if (!commitManager.saveCommit(commit)) {
        return false;
    }

    if (!headManager.updateHEAD(commit.getCommitID())) {
        return false;
    }

    indexManager.clearIndex(index);

    return true;
}

void RepositoryManager::status() const {
    indexManager.displayStagedFiles(index);
}
