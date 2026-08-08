// src/models/Index.cpp
#include "models/Index.h"

Index::Index()
    : stagedFiles{} {
}

HashTable& Index::getStagedFiles() {
    return stagedFiles;
}

const HashTable& Index::getStagedFiles() const {
    return stagedFiles;
}