#pragma once

#include <string>

#include "managers/IndexManager.h"
#include "managers/CommitManager.h"
#include "managers/BlobManager.h"
#include "managers/HeadManager.h"

#include "models/Index.h"

class RepositoryManager
{
private:

    IndexManager indexManager;

    CommitManager commitManager;

    BlobManager blobManager;

    HeadManager headManager;

    Index index;

    bool repositoryExists() const;

public:

    RepositoryManager();

    bool init();

    bool add(
        const std::string& filename,
        const std::string& content
    );

    bool commit(
        const std::string& message
    );

    void status() const;

    void log() const;

    bool checkout(
        const std::string& commitID
    );
};
