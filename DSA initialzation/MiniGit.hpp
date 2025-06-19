// src/MiniGit.hpp
#ifndef MINIGIT_HPP
#define MINIGIT_HPP

#include <string>
#include <filesystem>

class MiniGit {
public:
    MiniGit();               // Constructor
    ~MiniGit();              // Destructor
    void init();             // Initializes the repo

private:
    std::string repoDir;     // Name of the .minigit directory
};

#endif