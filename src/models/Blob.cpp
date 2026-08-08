// src/models/Blob.cpp
#include "models/Blob.h"

Blob::Blob()
    : hash{},
      filename{},
      content{},
      size{0},
      createdAt{0} {
}

Blob::Blob(
    const std::string& hashValue,
    const std::string& filenameValue,
    const std::string& contentValue,
    std::size_t sizeValue,
    std::time_t createdAtValue
)
    : hash(hashValue),
      filename(filenameValue),
      content(contentValue),
      size(sizeValue),
      createdAt(createdAtValue) {
}

const std::string& Blob::getHash() const {
    return hash;
}

const std::string& Blob::getFilename() const {
    return filename;
}

const std::string& Blob::getContent() const {
    return content;
}

std::size_t Blob::getSize() const {
    return size;
}

std::time_t Blob::getCreatedAt() const {
    return createdAt;
}