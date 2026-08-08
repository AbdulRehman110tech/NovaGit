// include/models/Blob.h
#pragma once

#include <string>
#include <cstddef>
#include <ctime>

class Blob {
public:
    Blob();

    Blob(
        const std::string& hashValue,
        const std::string& filenameValue,
        const std::string& contentValue,
        std::size_t sizeValue,
        std::time_t createdAtValue
    );

    const std::string& getHash() const;
    const std::string& getFilename() const;
    const std::string& getContent() const;
    std::size_t getSize() const;
    std::time_t getCreatedAt() const;

private:
    std::string hash;
    std::string filename;
    std::string content;
    std::size_t size;
    std::time_t createdAt;
};