// src/models/Commit.cpp
#include "models/Commit.h"

Commit::Commit()
    : commitID{},
      parentCommitID{},
      blobs{},
      message{},
      createdAt{0} {
}

Commit::Commit(
    const std::string& commitIDValue,
    const std::string& parentCommitIDValue,
    const std::vector<BlobReference>& blobsValue,
    const std::string& messageValue,
    std::time_t createdAtValue
)
    : commitID(commitIDValue),
      parentCommitID(parentCommitIDValue),
      blobs(blobsValue),
      message(messageValue),
      createdAt(createdAtValue) {
}

const std::string& Commit::getCommitID() const {
    return commitID;
}

const std::string& Commit::getParentCommitID() const {
    return parentCommitID;
}

const std::vector<BlobReference>& Commit::getBlobs() const {
    return blobs;
}

const std::string& Commit::getMessage() const {
    return message;
}

std::time_t Commit::getCreatedAt() const {
    return createdAt;
}