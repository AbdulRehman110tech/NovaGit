// include/models/Commit.h
#pragma once

#include <string>
#include <vector>
#include <ctime>
#include "BlobReference.h"

class Commit {
public:
    Commit();

    Commit(
        const std::string& commitIDValue,
        const std::string& parentCommitIDValue,
        const std::vector<BlobReference>& blobsValue,
        const std::string& messageValue,
        std::time_t createdAtValue
    );

    const std::string& getCommitID() const;
    const std::string& getParentCommitID() const;
    const std::vector<BlobReference>& getBlobs() const;
    const std::string& getMessage() const;
    std::time_t getCreatedAt() const;

private:
    std::string commitID;
    std::string parentCommitID;
    std::vector<BlobReference> blobs;
    std::string message;
    std::time_t createdAt;
};