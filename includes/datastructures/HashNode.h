// include/datastructures/HashNode.h
#pragma once

#include <string>

class HashNode {
public:
    HashNode();

    HashNode(
        const std::string& filenameValue,
        const std::string& blobHashValue
    );

    const std::string& getFilename() const;
    const std::string& getBlobHash() const;
    HashNode* getNext() const;

    void setBlobHash(const std::string& blobHashValue);
    void setNext(HashNode* nextNode);

private:
    std::string filename;
    std::string blobHash;
    HashNode* next;
};