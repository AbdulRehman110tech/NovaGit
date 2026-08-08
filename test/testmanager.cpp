#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

#include "managers/RepositoryManager.h"

namespace fs = std::filesystem;

int main()
{
    std::cout << "========================================\n";
    std::cout << "     REPOSITORY MANAGER TEST\n";
    std::cout << "========================================\n\n";

    bool testPassed = true;

    RepositoryManager repository;

    //------------------------------------
    // Initialize Repository
    //------------------------------------

    std::cout << "Initializing repository...\n";

    if (!repository.init())
    {
        std::cout << "[FAILED] Repository initialization failed.\n";
        testPassed = false;
    }
    else
    {
        std::cout << "[PASSED] Repository initialized.\n";
    }

    //------------------------------------
    // Verify Repository Structure
    //------------------------------------

    std::cout << "\nChecking repository structure...\n";

    if (fs::exists(".novagit") &&
        fs::exists(".novagit/objects") &&
        fs::exists(".novagit/commits") &&
        fs::exists(".novagit/HEAD") &&
        fs::exists(".novagit/index"))
    {
        std::cout << "[PASSED] Repository structure created.\n";
    }
    else
    {
        std::cout << "[FAILED] Repository structure missing.\n";
        testPassed = false;
    }

    //------------------------------------
    // Stage Files
    //------------------------------------

    std::cout << "\nAdding main.cpp...\n";

    if (!repository.add(
            "main.cpp",
            "int main(){ return 0; }"))
    {
        std::cout << "[FAILED] Could not stage main.cpp\n";
        testPassed = false;
    }
    else
    {
        std::cout << "[PASSED] main.cpp staged.\n";
    }

    std::cout << "\nAdding stack.cpp...\n";

    if (!repository.add(
            "stack.cpp",
            "class Stack{}"))
    {
        std::cout << "[FAILED] Could not stage stack.cpp\n";
        testPassed = false;
    }
    else
    {
        std::cout << "[PASSED] stack.cpp staged.\n";
    }

    //------------------------------------
    // Display Status
    //------------------------------------

    std::cout << "\nRepository Status\n";
    std::cout << "-------------------------\n";

    repository.status();

    //------------------------------------
    // Verify Index File
    //------------------------------------

    std::cout << "\nContents of .novagit/index\n";
    std::cout << "--------------------------\n";

    std::ifstream indexFile(".novagit/index");

    if (!indexFile)
    {
        std::cout << "[FAILED] Could not open index file.\n";
        testPassed = false;
    }
    else
    {
        std::string line;
        bool empty = true;

        while (std::getline(indexFile, line))
        {
            empty = false;
            std::cout << line << '\n';
        }

        if (empty)
        {
            std::cout << "[FAILED] Index file empty.\n";
            testPassed = false;
        }
        else
        {
            std::cout << "[PASSED] Index updated.\n";
        }
    }

    //------------------------------------
    // Commit
    //------------------------------------

    std::cout << "\nCreating commit...\n";

    if (!repository.commit("Initial Commit"))
    {
        std::cout << "[FAILED] Commit failed.\n";
        testPassed = false;
    }
    else
    {
        std::cout << "[PASSED] Commit created.\n";
    }

    //------------------------------------
    // Verify HEAD
    //------------------------------------

    std::cout << "\nContents of .novagit/HEAD\n";
    std::cout << "-------------------------\n";

    std::ifstream head(".novagit/HEAD");

    if (!head)
    {
        std::cout << "[FAILED] Could not open HEAD.\n";
        testPassed = false;
    }
    else
    {
        std::string commitID;
        std::getline(head, commitID);

        std::cout << commitID << '\n';

        if (commitID.empty())
        {
            std::cout << "[FAILED] HEAD is empty.\n";
            testPassed = false;
        }
        else
        {
            std::cout << "[PASSED] HEAD updated.\n";
        }
    }

    //------------------------------------
    // Verify Commit Directory
    //------------------------------------

    std::cout << "\nChecking commit files...\n";

    bool foundCommit = false;

    for (const auto& entry :
         fs::directory_iterator(".novagit/commits"))
    {
        foundCommit = true;

        std::cout << "Commit File : "
                  << entry.path().filename().string()
                  << '\n';
    }

    if (!foundCommit)
    {
        std::cout << "[FAILED] No commit file found.\n";
        testPassed = false;
    }
    else
    {
        std::cout << "[PASSED] Commit file created.\n";
    }

    //------------------------------------
    // Verify Objects
    //------------------------------------

    std::cout << "\nChecking object files...\n";

    bool foundObject = false;

    for (const auto& entry :
         fs::directory_iterator(".novagit/objects"))
    {
        foundObject = true;

        std::cout << "Object : "
                  << entry.path().filename().string()
                  << '\n';
    }

    if (!foundObject)
    {
        std::cout << "[FAILED] No object files found.\n";
        testPassed = false;
    }
    else
    {
        std::cout << "[PASSED] Object files created.\n";
    }

    //------------------------------------
    // Verify Index Cleared
    //------------------------------------

    std::cout << "\nChecking index after commit...\n";

    std::ifstream cleared(".novagit/index");

    std::string line;
    bool empty = true;

    while (std::getline(cleared, line))
    {
        empty = false;
    }

    if (empty)
    {
        std::cout << "[PASSED] Index cleared after commit.\n";
    }
    else
    {
        std::cout << "[FAILED] Index still contains staged files.\n";
        testPassed = false;
    }

    //------------------------------------
    // Final Result
    //------------------------------------

    std::cout << "\n========================================\n";

    if (testPassed)
    {
        std::cout << "ALL REPOSITORY MANAGER TESTS PASSED\n";
    }
    else
    {
        std::cout << "REPOSITORY MANAGER TESTS FAILED\n";
    }

    std::cout << "========================================\n";

    return 0;
}