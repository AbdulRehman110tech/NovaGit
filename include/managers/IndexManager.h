#pragma once

#include <string>

#include "managers/BlobManager.h"
#include "models/Index.h"

class IndexManager
{
private:

    BlobManager blobManager;

    bool saveIndexToDisk(
        const Index& index
    ) const;

public:

    IndexManager();

    bool loadIndexFromDisk(
        Index& index
    );

    bool stageFile(
        Index& index,
        const std::string& filename,
        const std::string& content
    );

    bool unstageFile(
        Index& index,
        const std::string& filename
    );

    bool isStaged(
        const Index& index,
        const std::string& filename
    ) const;

    void clearIndex(
        Index& index
    );

    void displayStagedFiles(
        const Index& index
    ) const;
};
