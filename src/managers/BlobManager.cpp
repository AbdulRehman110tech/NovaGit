// src/managers/BlobManager.cpp
#include <ctime>
#include <filesystem>
#include <fstream>
#include <string>

#include "managers/BlobManager.h"

namespace fs = std::filesystem;

namespace {

constexpr char kObjectStoreDirectory[] = ".novagit/objects";

std::string objectStorePath(
    const std::string& hash
) {
    return std::string(kObjectStoreDirectory) + "/" + hash;
}

void writeBlobField(
    std::ofstream& out,
    const std::string& label,
    const std::string& value
) {
    out << label << ':' << value << '\n';
}

} // namespace

BlobManager::BlobManager()
    : hashManager{} {
}

Blob BlobManager::createBlob(
    const std::string& filename,
    const std::string& content
) {
    const std::string hash = hashManager.generateHash(content);

    return Blob{
        hash,
        filename,
        content,
        content.size(),
        std::time(nullptr)
    };
}

bool BlobManager::saveBlob(
    const Blob& blob
) {
    const std::string hash = blob.getHash();

    if (blobExists(hash)) {
        return true;
    }

    std::ofstream out(objectStorePath(hash), std::ios::out);

    if (!out) {
        return false;
    }

    writeBlobField(out, "Hash", blob.getHash());
    writeBlobField(out, "Filename", blob.getFilename());
    writeBlobField(out, "Size", std::to_string(blob.getSize()));
    writeBlobField(out, "Timestamp", std::to_string(blob.getCreatedAt()));

    out << "Content:\n";
    out << blob.getContent() << '\n';

    return out.good();
}

bool BlobManager::blobExists(
    const std::string& hash
) const {
    return fs::exists(objectStorePath(hash));
}
