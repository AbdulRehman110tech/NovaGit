// src/managers/CommitManager.cpp
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "managers/CommitManager.h"

namespace fs = std::filesystem;

namespace {

constexpr char kCommitStoreDirectory[] = ".novagit/commits";

std::string commitStorePath(
    const std::string& commitID
) {
    return std::string(kCommitStoreDirectory) + "/" + commitID;
}

std::string buildCommitID(
    const HashManager& hashManager,
    const std::string& parentCommitID,
    const std::string& message,
    std::time_t timestamp
) {
    return hashManager.generateHash(
        parentCommitID + message + std::to_string(timestamp)
    );
}

} // namespace

CommitManager::CommitManager()
    : hashManager{} {
}

Commit CommitManager::createCommit(
    Index& index,
    const std::string& parentCommitID,
    const std::string& message
) {
    const std::vector<BlobReference> blobs =
        index.getStagedFiles().getAllBlobReferences();

    const std::time_t timestamp = std::time(nullptr);

    const std::string commitID = buildCommitID(
        hashManager,
        parentCommitID,
        message,
        timestamp
    );

    return Commit{
        commitID,
        parentCommitID,
        blobs,
        message,
        timestamp
    };
}

bool CommitManager::saveCommit(
    const Commit& commit
) {
    if (commitExists(commit.getCommitID())) {
        return true;
    }

    std::ofstream out(commitStorePath(commit.getCommitID()), std::ios::out);

    if (!out) {
        return false;
    }

    out << "CommitID:" << commit.getCommitID() << '\n';
    out << '\n';
    out << "ParentCommitID:" << commit.getParentCommitID() << '\n';
    out << '\n';
    out << "Message:" << commit.getMessage() << '\n';
    out << '\n';
    out << "Timestamp:" << commit.getCreatedAt() << '\n';
    out << '\n';
    out << "Files:\n";

    const std::vector<BlobReference>& blobs = commit.getBlobs();

    for (const BlobReference& reference : blobs) {
        out << reference.filename << ' ' << reference.blobHash << '\n';
    }

    return out.good();
}

bool CommitManager::commitExists(
    const std::string& commitID
) const {
    return fs::exists(commitStorePath(commitID));
}

void CommitManager::displayCommit(
    const Commit& commit
) const {
    std::cout << "----------------------------------------\n";
    std::cout << "Commit ID: " << commit.getCommitID() << '\n';
    std::cout << "Parent : " << commit.getParentCommitID() << '\n';
    std::cout << "Message : " << commit.getMessage() << '\n';
    std::cout << "Timestamp : " << commit.getCreatedAt() << '\n';
    std::cout << "Files\n";

    const std::vector<BlobReference>& blobs = commit.getBlobs();

    for (const BlobReference& reference : blobs) {
        std::cout << reference.filename << " -> " << reference.blobHash << '\n';
    }

    std::cout << "----------------------------------------\n";
}
