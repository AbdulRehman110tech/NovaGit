// include/models/Index.h
#pragma once

#include "../datastructures/HashTable.h"

class Index {
public:
    Index();

    HashTable& getStagedFiles();
    const HashTable& getStagedFiles() const;

private:
    HashTable stagedFiles;
};