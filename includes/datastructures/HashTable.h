// include/datastructures/HashTable.h
#pragma once

#include <vector>
#include <string>
#include <cstddef>

#include "HashNode.h"
#include "../models/BlobReference.h"

class HashTable {
public:
    HashTable();
    ~HashTable();

    void insert(
        const std::string& filename,
        const std::string& blobHash
    );

    bool update(
        const std::string& filename,
        const std::string& newBlobHash
    );

    bool remove(const std::string& filename);

    bool contains(const std::string& filename) const;

    std::string getBlobHash(const std::string& filename) const;

    void clear();

    bool empty() const;

    std::size_t size() const;

    void display() const;

    std::vector<BlobReference> getAllBlobReferences() const;

private:
    std::vector<HashNode*> buckets;

    std::size_t bucketCount;

    std::size_t currentSize;

    std::size_t hashFunction(const std::string& filename) const;
};