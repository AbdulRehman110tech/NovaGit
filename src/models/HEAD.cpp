// src/models/HEAD.cpp
#include "models/HEAD.h"

HEAD::HEAD()
    : currentCommitID{} {
}

HEAD::HEAD(const std::string& currentCommitIDValue)
    : currentCommitID(currentCommitIDValue) {
}

const std::string& HEAD::getCurrentCommitID() const {
    return currentCommitID;
}

void HEAD::setCurrentCommitID(const std::string& currentCommitIDValue) {
    currentCommitID = currentCommitIDValue;
}