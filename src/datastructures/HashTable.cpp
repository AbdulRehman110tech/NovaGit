// src/datastructures/HashTable.cpp
#include "datastructures/HashTable.h"

#include <iostream>

HashTable::HashTable()
    : buckets(101, nullptr),
      bucketCount(101),
      currentSize(0) {
}

HashTable::~HashTable() {
    clear();
}

void HashTable::insert(
    const std::string& filename,
    const std::string& blobHash
) {
    const std::size_t index = hashFunction(filename);

    HashNode* current = buckets[index];

    while (current != nullptr) {
        if (current->getFilename() == filename) {
            if (current->getBlobHash() != blobHash) {
                current->setBlobHash(blobHash);
            }

            return;
        }

        current = current->getNext();
    }

    HashNode* newNode = new HashNode(filename, blobHash);
    newNode->setNext(buckets[index]);
    buckets[index] = newNode;

    ++currentSize;
}

bool HashTable::update(
    const std::string& filename,
    const std::string& newBlobHash
) {
    const std::size_t index = hashFunction(filename);

    HashNode* current = buckets[index];

    while (current != nullptr) {
        if (current->getFilename() == filename) {
            current->setBlobHash(newBlobHash);
            return true;
        }

        current = current->getNext();
    }

    return false;
}

bool HashTable::remove(const std::string& filename) {
    const std::size_t index = hashFunction(filename);

    HashNode* current = buckets[index];
    HashNode* previous = nullptr;

    while (current != nullptr) {
        if (current->getFilename() == filename) {
            if (previous == nullptr) {
                buckets[index] = current->getNext();
            } else {
                previous->setNext(current->getNext());
            }

            delete current;
            --currentSize;

            return true;
        }

        previous = current;
        current = current->getNext();
    }

    return false;
}

bool HashTable::contains(const std::string& filename) const {
    const std::size_t index = hashFunction(filename);

    const HashNode* current = buckets[index];

    while (current != nullptr) {
        if (current->getFilename() == filename) {
            return true;
        }

        current = current->getNext();
    }

    return false;
}

std::string HashTable::getBlobHash(const std::string& filename) const {
    const std::size_t index = hashFunction(filename);

    const HashNode* current = buckets[index];

    while (current != nullptr) {
        if (current->getFilename() == filename) {
            return current->getBlobHash();
        }

        current = current->getNext();
    }

    return "";
}

void HashTable::clear() {
    for (std::size_t i = 0; i < bucketCount; ++i) {
        HashNode* current = buckets[i];

        while (current != nullptr) {
            HashNode* next = current->getNext();
            delete current;
            current = next;
        }

        buckets[i] = nullptr;
    }

    currentSize = 0;
}

bool HashTable::empty() const {
    return currentSize == 0;
}

std::size_t HashTable::size() const {
    return currentSize;
}

void HashTable::display() const {
    for (std::size_t i = 0; i < bucketCount; ++i) {
        std::cout << "Bucket " << i << ": ";

        const HashNode* current = buckets[i];

        if (current == nullptr) {
            std::cout << "empty";
        } else {
            while (current != nullptr) {
                std::cout << "["
                          << current->getFilename()
                          << " -> "
                          << current->getBlobHash()
                          << "]";

                if (current->getNext() != nullptr) {
                    std::cout << " -> ";
                }

                current = current->getNext();
            }
        }

        std::cout << '\n';
    }
}

std::vector<BlobReference> HashTable::getAllBlobReferences() const {
    std::vector<BlobReference> blobReferences;
    blobReferences.reserve(currentSize);

    for (std::size_t i = 0; i < bucketCount; ++i) {
        const HashNode* current = buckets[i];

        while (current != nullptr) {
            BlobReference reference;
            reference.filename = current->getFilename();
            reference.blobHash = current->getBlobHash();

            blobReferences.push_back(reference);

            current = current->getNext();
        }
    }

    return blobReferences;
}

std::size_t HashTable::hashFunction(const std::string& filename) const {
    std::size_t hash = 0;
    const std::size_t prime = 31;

    for (char character : filename) {
        hash = hash * prime + static_cast<std::size_t>(static_cast<unsigned char>(character));
    }

    return hash % bucketCount;
}