// includes/managers/BlobManager.h
#pragma once

#include <string>

#include "managers/HashManager.h"
#include "models/Blob.h"

class BlobManager {
public:
    BlobManager();

    Blob createBlob(
        const std::string& filename,
        const std::string& content
    );

    bool saveBlob(
        const Blob& blob
    );

    bool blobExists(
        const std::string& hash
    ) const;

private:
    HashManager hashManager;
};
