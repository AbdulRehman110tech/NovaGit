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
      blobManager{},
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

void RepositoryManager::log() const {
    std::string currentCommitID = headManager.getCurrentCommitID();

    while (!currentCommitID.empty()) {
        Commit commit = commitManager.loadCommit(currentCommitID);

        commitManager.displayCommit(commit);

        currentCommitID = commit.getParentCommitID();
    }
}

bool RepositoryManager::checkout(
    const std::string& commitID
) {
    if (!commitManager.commitExists(commitID)) {
        return false;
    }

    Commit commit = commitManager.loadCommit(commitID);

    const std::vector<BlobReference>& blobs = commit.getBlobs();

    for (const BlobReference& reference : blobs) {
        Blob blob = blobManager.loadBlob(reference.blobHash);

        if (blob.getHash().empty()) {
            return false;
        }

        std::ofstream out(reference.filename, std::ios::out | std::ios::trunc);

        if (!out) {
            return false;
        }

        out << blob.getContent();

        if (!out.good()) {
            return false;
        }
    }

    if (!headManager.updateHEAD(commitID)) {
        return false;
    }

    indexManager.clearIndex(index);

    return true;
}
