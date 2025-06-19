// src/MiniGit.cpp
#include "MiniGit.hpp"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

MiniGit::MiniGit() {
    repoDir = ".minigit"; // default repo folder
}

MiniGit::~MiniGit() {}  //destuctor currently empty,but defined for completeness/future use

void MiniGit::init() {
    if (!fs::exists(repoDir)) {
        fs::create_directory(repoDir);
        std::cout << "MiniGit repository initialized in '" << repoDir << "'\n";
    } else {
        std::cout << "Repository already exists at '" << repoDir << "'\n";
    }
}

//Checkout:restores files from specific commit ID
void MiniGit::checkout(const std::string& commitID) {
    //checking if the commit exists
    if(commitMap.find(commitID) == commitMap.end()) {
        std::cerr << "Commit ID not found.\n";
        return;
    }

    //get the commit to restore
    const Commit commit = commitMap[commitID];

    //call helper function to restore files
    restoreFiles(commit);

    //update the current commit ID
    currentCommitID = commitID;

    std::cout << "Checked out to commit " << commitID << ".\n";
}

//merge:merges another branch into the current branch
void MiniGit::merge(const std::string& branchName) {
    //checking if the branch exists
    if (branches.find(branchName) == branches.end()) {
        std::cerr << "Branch not found.\n";
        return;
    }

    //get the commit ID of the other branch 
    std::string otherCommitID = branches[branchName];

    //get both commits
    Commit& currentCommit = commitMap[currentCommitID];
    Commit& otherCommit = commitMap[otherCommitID];

    // Call conflict resolution helper
    resolveConflicts(currentCommit, otherCommit);

    std::cout << "Merged branch '" << branchName << "' into '" << currentBranch << "'.\n";
}

// Helper: Restores files from a given commit
void MiniGit::restoreFiles(const Commit& commit) {
    for (const auto& [filename, fileHash] : commit.fileSnapshots) {
        std::string filePath = repoDir + "/" + fileHash;

        // If the versioned file exists, copy it back to working directory
        if (fs::exists(filePath)) {
            fs::copy_file(filePath, filename, fs::copy_options::overwrite_existing);
        } else {
            std::cerr << "File " << fileHash << " not found in repository.\n";
        }
    }
}

// Helper: Simple merge conflict resolution logic (basic example)
void MiniGit::resolveConflicts(const Commit& current, const Commit& other) {
    for (const auto& [filename, otherVersion] : other.fileSnapshots) {
        const auto& currentIt = current.fileSnapshots.find(filename);

        // If file exists in both commits but versions differ
        if (currentIt != current.fileSnapshots.end() && currentIt->second != otherVersion) {
            std::cout << "Conflict detected in file: " << filename << "\n";
            std::cout << "Manual merge required (placeholder logic).\n";

            // Basic strategy: keep both versions (you can improve this)
            std::string mergedFilename = filename + ".merged";
            fs::copy_file(repoDir + "/" + otherVersion, mergedFilename,
                          fs::copy_options::overwrite_existing);
        }
    }
}

// Helper: Generates a dummy commit ID (you can improve with hashes or timestamps)
std::string MiniGit::generateCommitID() {
    static int counter = 0;
    return "commit" + std::to_string(++counter);
}
