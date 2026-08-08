// src/managers/CommitManager.cpp
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "managers/CommitManager.h"

namespace fs = std::filesystem;

namespace {

constexpr char kCommitStoreDirectory[] = ".novagit/commits";

constexpr char kCommitIDPrefix[] = "CommitID:";

constexpr char kParentCommitIDPrefix[] = "ParentCommitID:";

constexpr char kMessagePrefix[] = "Message:";

constexpr char kTimestampPrefix[] = "Timestamp:";

constexpr char kFilesLine[] = "Files:";

std::string commitStorePath(
    const std::string& commitID
) {
    return std::string(kCommitStoreDirectory) + "/" + commitID;
}

std::string stripValuePrefix(
    const std::string& line,
    const std::string& prefix
) {
    if (line.compare(0, prefix.size(), prefix) != 0) {
        return std::string{};
    }

    return line.substr(prefix.size());
}

BlobReference parseBlobReference(
    const std::string& line
) {
    BlobReference reference;

    std::istringstream stream(line);

    stream >> reference.filename;
    stream >> reference.blobHash;

    return reference;
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

Commit CommitManager::loadCommit(
    const std::string& commitID
) const {
    std::ifstream in(commitStorePath(commitID));

    if (!in) {
        return Commit{};
    }

    std::string loadedCommitID;
    std::string parentCommitID;
    std::string message;
    std::time_t timestamp = 0;

    std::vector<BlobReference> blobs;

    bool readingFiles = false;

    std::string line;

    while (std::getline(in, line)) {
        if (line.empty()) {
            continue;
        }

        if (line == kFilesLine) {
            readingFiles = true;
            continue;
        }

        if (readingFiles) {
            blobs.push_back(parseBlobReference(line));
            continue;
        }

        const std::string commitIDValue = stripValuePrefix(
            line,
            kCommitIDPrefix
        );

        if (!commitIDValue.empty()) {
            loadedCommitID = commitIDValue;
            continue;
        }

        const std::string parentCommitIDValue = stripValuePrefix(
            line,
            kParentCommitIDPrefix
        );

        if (!parentCommitIDValue.empty()) {
            parentCommitID = parentCommitIDValue;
            continue;
        }

        const std::string messageValue = stripValuePrefix(
            line,
            kMessagePrefix
        );

        if (!messageValue.empty()) {
            message = messageValue;
            continue;
        }

        const std::string timestampValue = stripValuePrefix(
            line,
            kTimestampPrefix
        );

        if (!timestampValue.empty()) {
            std::istringstream stream(timestampValue);
            stream >> timestamp;
        }
    }

    return Commit{
        loadedCommitID,
        parentCommitID,
        blobs,
        message,
        timestamp
    };
}
