#pragma once

#include <string>

class HeadManager
{
public:

    HeadManager();

    std::string getCurrentCommitID() const;

    bool updateHEAD(
        const std::string& commitID
    );

    bool isInitialized() const;
};
