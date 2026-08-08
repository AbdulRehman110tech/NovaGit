// include/models/HEAD.h
#pragma once

#include <string>

class HEAD {
public:
    HEAD();

    explicit HEAD(const std::string& currentCommitIDValue);

    const std::string& getCurrentCommitID() const;

    void setCurrentCommitID(const std::string& currentCommitIDValue);

private:
    std::string currentCommitID;
};