#include <iostream>

#include "parser/CommandParser.h"

int main()
{
    std::cout << "========================================\n";
    std::cout << "              NOVAGIT\n";
    std::cout << "      Mini Version Control System\n";
    std::cout << "========================================\n\n";

    std::cout << "Name       : Abdul Rehman\n";
    std::cout << "Student ID : Bscs24149\n\n";

    std::cout << "Project Story\n";
    std::cout << "-------------\n";
    std::cout << "NovaGit is a mini version control system developed\n";
    std::cout << "in C++ to understand the internal working of Git.\n\n";

    std::cout << "The project was developed by designing and testing\n";
    std::cout << "individual components such as hashing, blob storage,\n";
    std::cout << "staging, commits, HEAD management, and repository\n";
    std::cout << "management before integrating them into a complete\n";
    std::cout << "command-line application.\n\n";

    std::cout << "NovaGit supports repository initialization, file\n";
    std::cout << "staging, commits, status, commit history, and\n";
    std::cout << "checkout functionality.\n\n";

    std::cout << "========================================\n";
    std::cout << "Type 'help' to see available commands.\n";
    std::cout << "========================================\n\n";

    CommandParser parser;

    parser.run();

    return 0;
}