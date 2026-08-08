// src/datastructures/HashNode.cpp
#include "datastructures/HashNode.h"

HashNode::HashNode()
    : filename{},
      blobHash{},
      next{nullptr} {
}

HashNode::HashNode(
    const std::string& filenameValue,
    const std::string& blobHashValue
)
    : filename(filenameValue),
      blobHash(blobHashValue),
      next{nullptr} {
}

const std::string& HashNode::getFilename() const {
    return filename;
}

const std::string& HashNode::getBlobHash() const {
    return blobHash;
}

HashNode* HashNode::getNext() const {
    return next;
}

void HashNode::setBlobHash(const std::string& blobHashValue) {
    blobHash = blobHashValue;
}

void HashNode::setNext(HashNode* nextNode) {
    next = nextNode;
}