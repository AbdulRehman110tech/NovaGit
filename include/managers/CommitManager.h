#pragma once

#include <string>

#include "managers/HashManager.h"
#include "models/Commit.h"
#include "models/Index.h"

class CommitManager
{
private:

    HashManager hashManager;

public:

    CommitManager();

    Commit createCommit(
        Index& index,
        const std::string& parentCommitID,
        const std::string& message
    );

    bool saveCommit(
        const Commit& commit
    );

    bool commitExists(
        const std::string& commitID
    ) const;

    void displayCommit(
        const Commit& commit
    ) const;

    Commit loadCommit(
        const std::string& commitID
    ) const;
};
