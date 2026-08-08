#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "parser/CommandParser.h"

namespace {

std::string trim(
    const std::string& value
) {
    const std::size_t first = value.find_first_not_of(" \t\r\n");

    if (first == std::string::npos) {
        return std::string{};
    }

    const std::size_t last = value.find_last_not_of(" \t\r\n");

    return value.substr(first, last - first + 1);
}

} // namespace

CommandParser::CommandParser()
    : repository{}
{
}

void CommandParser::run()
{
    std::string command;

    while (true)
    {
        std::cout << "NovaGit> ";

        if (!std::getline(std::cin, command))
        {
            std::cout << '\n';
            break;
        }

        if (!processCommand(command))
        {
            break;
        }
    }
}

bool CommandParser::processCommand(
    const std::string& command
)
{
    const std::string trimmedCommand = trim(command);

    if (trimmedCommand == "exit" || trimmedCommand == "quit")
    {
        return false;
    }

    if (trimmedCommand == "help")
    {
        showHelp();
        return true;
    }

    std::istringstream stream(trimmedCommand);

    std::string token;

    if (!(stream >> token) || token != "novagit")
    {
        showUnknownCommand();
        return true;
    }

    if (!(stream >> token))
    {
        std::cout << "No command specified.\n";
        std::cout << "Type\n";
        std::cout << "help\n";
        std::cout << "for available commands.\n";
        return true;
    }

    //------------------------------------
    // init
    //------------------------------------

    if (token == "init")
    {
        if (repository.init())
        {
            std::cout << "Repository initialized successfully.\n";
        }
        else
        {
            std::cout << "Failed to initialize repository.\n";
        }

        return true;
    }

    //------------------------------------
    // add
    //------------------------------------

    if (token == "add")
    {
        std::string filename;

        if (!(stream >> filename))
        {
            std::cout << "Usage: novagit add <filename>\n";
            return true;
        }

        std::ifstream in(filename);

        if (!in)
        {
            std::cout << "Error: could not open file '"
                      << filename << "'.\n";
            return true;
        }

        std::ostringstream buffer;
        buffer << in.rdbuf();

        const std::string content = buffer.str();

        if (repository.add(filename, content))
        {
            std::cout << "Staged " << filename << ".\n";
        }
        else
        {
            std::cout << "Failed to stage " << filename << ".\n";
        }

        return true;
    }

    //------------------------------------
    // commit
    //------------------------------------

    if (token == "commit")
    {
        std::string flag;

        if (!(stream >> flag) || flag != "-m")
        {
            std::cout << "Usage: novagit commit -m \"message\"\n";
            return true;
        }

        const std::size_t firstQuote = trimmedCommand.find('"');
        const std::size_t lastQuote = trimmedCommand.find_last_of('"');

        if (firstQuote == std::string::npos ||
            lastQuote == firstQuote)
        {
            std::cout << "Usage: novagit commit -m \"message\"\n";
            return true;
        }

        const std::string message =
            trimmedCommand.substr(
                firstQuote + 1,
                lastQuote - firstQuote - 1
            );

        if (repository.commit(message))
        {
            std::cout << "Commit created successfully.\n";
        }
        else
        {
            std::cout << "Failed to create commit.\n";
        }

        return true;
    }

    //------------------------------------
    // status
    //------------------------------------

    if (token == "status")
    {
        repository.status();
        return true;
    }

    //------------------------------------
    // log
    //------------------------------------

    if (token == "log")
    {
        repository.log();
        return true;
    }

    //------------------------------------
    // checkout
    //------------------------------------

    if (token == "checkout")
    {
        std::string commitID;

        if (!(stream >> commitID))
        {
            std::cout << "Usage: novagit checkout <commitID>\n";
            return true;
        }

        if (repository.checkout(commitID))
        {
            std::cout << "Checked out commit "
                      << commitID
                      << ".\n";
        }
        else
        {
            std::cout << "Failed to check out commit "
                      << commitID
                      << ".\n";
        }

        return true;
    }

    showUnknownCommand();

    return true;
}

void CommandParser::showUnknownCommand() const
{
    std::cout << "Unknown command.\n";
    std::cout << "Type\n";
    std::cout << "help\n";
    std::cout << "for available commands.\n";
}

void CommandParser::showHelp() const
{
    std::cout << "Available Commands\n";
    std::cout << "novagit init\n";
    std::cout << "novagit add <filename>\n";
    std::cout << "novagit commit -m \"message\"\n";
    std::cout << "novagit status\n";
    std::cout << "novagit log\n";
    std::cout << "novagit checkout <commitID>\n";
    std::cout << "help\n";
    std::cout << "exit\n";
    std::cout << "quit\n";
}