// src/managers/BlobManager.cpp
#include <ctime>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "managers/BlobManager.h"

namespace fs = std::filesystem;

namespace {

constexpr char kObjectStoreDirectory[] = ".novagit/objects";

constexpr char kHashPrefix[] = "Hash:";

constexpr char kFilenamePrefix[] = "Filename:";

constexpr char kSizePrefix[] = "Size:";

constexpr char kTimestampPrefix[] = "Timestamp:";

constexpr char kContentLine[] = "Content:";

std::string objectStorePath(
    const std::string& hash
) {
    return std::string(kObjectStoreDirectory) + "/" + hash;
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

Blob BlobManager::loadBlob(
    const std::string& hash
) const {
    std::ifstream in(objectStorePath(hash));

    if (!in) {
        return Blob{};
    }

    std::string loadedHash;
    std::string filename;
    std::size_t size = 0;
    std::time_t timestamp = 0;

    std::string content;

    bool readingContent = false;

    std::string line;

    while (std::getline(in, line)) {
        if (readingContent) {
            if (!content.empty()) {
                content += '\n';
            }

            content += line;
            continue;
        }

        const std::string hashValue = stripValuePrefix(
            line,
            kHashPrefix
        );

        if (!hashValue.empty()) {
            loadedHash = hashValue;
            continue;
        }

        const std::string filenameValue = stripValuePrefix(
            line,
            kFilenamePrefix
        );

        if (!filenameValue.empty()) {
            filename = filenameValue;
            continue;
        }

        const std::string sizeValue = stripValuePrefix(
            line,
            kSizePrefix
        );

        if (!sizeValue.empty()) {
            std::istringstream stream(sizeValue);
            stream >> size;
            continue;
        }

        const std::string timestampValue = stripValuePrefix(
            line,
            kTimestampPrefix
        );

        if (!timestampValue.empty()) {
            std::istringstream stream(timestampValue);
            stream >> timestamp;
            continue;
        }

        if (line == kContentLine) {
            readingContent = true;
        }
    }

    return Blob{
        loadedHash,
        filename,
        content,
        size,
        timestamp
    };
}
