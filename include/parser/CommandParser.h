#pragma once

#include <string>

#include "managers/RepositoryManager.h"

class CommandParser
{
private:

    RepositoryManager repository;

    bool processCommand(
        const std::string& command
    );

    void showHelp() const;

    void showUnknownCommand() const;

public:

    CommandParser();

    void run();
};