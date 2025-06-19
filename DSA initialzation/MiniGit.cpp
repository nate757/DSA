// src/MiniGit.cpp
#include "MiniGit.hpp"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

MiniGit::MiniGit() {
    repoDir = ".minigit"; // default repo folder
}

MiniGit::~MiniGit() {}

void MiniGit::init() {
    if (!fs::exists(repoDir)) {
        fs::create_directory(repoDir);
        std::cout << "MiniGit repository initialized in '" << repoDir << "'\n";
    } else {
        std::cout << "Repository already exists at '" << repoDir << "'\n";
    }
}
