// src/managers/IndexManager.cpp
#include <fstream>
#include <string>
#include <vector>

#include "managers/IndexManager.h"

namespace {

constexpr char kIndexFilePath[] = ".novagit/index";

} // namespace

IndexManager::IndexManager()
    : blobManager{} {
}

bool IndexManager::stageFile(
    Index& index,
    const std::string& filename,
    const std::string& content
) {
    Blob blob = blobManager.createBlob(filename, content);

    if (!blobManager.saveBlob(blob)) {
        return false;
    }

    index.getStagedFiles().insert(
        filename,
        blob.getHash()
    );

    return saveIndexToDisk(index);
}

bool IndexManager::unstageFile(
    Index& index,
    const std::string& filename
) {
    if (!index.getStagedFiles().remove(filename)) {
        return false;
    }

    return saveIndexToDisk(index);
}

bool IndexManager::isStaged(
    const Index& index,
    const std::string& filename
) const {
    return index.getStagedFiles().contains(filename);
}

void IndexManager::clearIndex(
    Index& index
) {
    index.getStagedFiles().clear();
    saveIndexToDisk(index);
}

void IndexManager::displayStagedFiles(
    const Index& index
) const {
    index.getStagedFiles().display();
}

bool IndexManager::saveIndexToDisk(
    const Index& index
) const {
    std::ofstream out(kIndexFilePath, std::ios::out | std::ios::trunc);

    if (!out) {
        return false;
    }

    const std::vector<BlobReference>& references =
        index.getStagedFiles().getAllBlobReferences();

    for (const BlobReference& reference : references) {
        out << reference.filename << ' ' << reference.blobHash << '\n';
    }

    return out.good();
}

bool IndexManager::loadIndexFromDisk(
    Index& index
) {
    std::ifstream in(kIndexFilePath);

    if (!in) {
        return false;
    }

    index.getStagedFiles().clear();

    std::string filename;
    std::string blobHash;

    while (in >> filename >> blobHash) {
        index.getStagedFiles().insert(
            filename,
            blobHash
        );
    }

    return true;
}
