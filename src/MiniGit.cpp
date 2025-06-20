
#include "MiniGit.hpp"
#include <iostream>
#include <filesystem>
#include<fstream>

namespace fs = std::filesystem;

MiniGit::MiniGit() {
    repoDir = ".minigit"; //  this is our default repo folder
}

MiniGit::~MiniGit() {}  //destuctor
// Initialisor tha prepares our repository
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

//  This is a Helper function that restores files from a given commit
void MiniGit::restoreFiles(const Commit& commit) {
    for (const auto& [filename, fileHash] : commit.fileSnapshots) {
        std::string versionPath = repoDir + "/" + fileHash;

        try {
            if (!fs::exists(versionPath)) {
                std::cerr << "⚠️  Version file not found: " << versionPath << "\n";
                continue;
            }

            //  We have to ensure we safely overwrite the existing file
            if (fs::exists(filename)) {
                fs::remove(filename);
            }

            fs::copy_file(versionPath, filename);
            std::cout << "✅ Restored: " << filename << "\n";
        } catch (const fs::filesystem_error& e) {
            std::cerr << "❌ Error restoring " << filename << ": " << e.what() << "\n";
        }
    }
}

// This is a  helper function that helps in  Simple merge conflict resolution logic
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
// Adder that stages the files we want to commit
void MiniGit::add(const std::string& fileName) {
    if (!fs::exists(fileName)) {
        std::cerr << "File '" << fileName << "' does not exist.\n";
        return;
    }

    // For simplicity, we'll use file name + timestamp hash-like string
    std::string hash = fileName + "_" + std::to_string(std::time(nullptr));
    std::string versionPath = repoDir + "/" + hash;

    // Copy current version of file to .minigit
    fs::copy_file(fileName, versionPath, fs::copy_options::overwrite_existing);

    // Stage it
    stagingArea[fileName] = hash;
    std::cout << "Staged '" << fileName << "' for commit.\n";
}
// Commites our files that we staged earlier with a message
void MiniGit::commit(const std::string& message) {
    if (stagingArea.empty()) {
        std::cout << "No files staged. Use `add` before committing.\n";
        return;
    }

    Commit newCommit;
    newCommit.id = generateCommitID();
    newCommit.parentID = currentCommitID;

    // Copy staged versions into new commit
    for (const auto& [fileName, version] : stagingArea) {
        newCommit.fileSnapshots[fileName] = version;
    }

    // Store the commit
    commitMap[newCommit.id] = newCommit;
    currentCommitID = newCommit.id;
    branches[currentBranch] = currentCommitID;

    stagingArea.clear();  // Reset staging

    std::cout << "✅ Commit '" << currentCommitID << "' created with message: " << message << "\n";
}
// Log or list of commites we made so far
void MiniGit::log() const {
    std::string commitID = currentCommitID;

    std::cout << "🔗 Commit log for branch '" << currentBranch << "':\n";
    while (!commitID.empty()) {
        const Commit& c = commitMap.at(commitID);
        std::cout << "- " << c.id << " (parent: " << c.parentID << ")\n";
        commitID = c.parentID;
    }
}
// Different branches to work on
void MiniGit::branch(const std::string& branchName) {
    if (branches.count(branchName)) {
        std::cerr << "Branch '" << branchName << "' already exists.\n";
        return;
    }

    branches[branchName] = currentCommitID;
    std::cout << "🌿 Created branch '" << branchName << "' from commit " << currentCommitID << ".\n";
}
// helper of branch
void MiniGit::checkoutBranch(const std::string& branchName) {
    if (!branches.count(branchName)) {
        std::cerr << "Branch '" << branchName << "' does not exist.\n";
        return;
    }

    currentBranch = branchName;
    currentCommitID = branches[branchName];
    std::cout << "🔁 Switched to branch '" << currentBranch << "'\n";

    if (!currentCommitID.empty()) {
        restoreFiles(commitMap[currentCommitID]);
    }
}
// Special feature that compares two commits made at a different time line by line and givrd us the difference
void MiniGit::diff(const std::string& fileName) {
    if (currentCommitID.empty()) {
        std::cout << "No commits yet. Nothing to diff.\n";
        return;
    }

    const Commit& lastCommit = commitMap[currentCommitID];
    auto it = lastCommit.fileSnapshots.find(fileName);

    if (it == lastCommit.fileSnapshots.end()) {
        std::cout << "File not found in last commit. Nothing to compare.\n";
        return;
    }

    std::string lastVersionPath = repoDir + "/" + it->second;

    std::ifstream committedFile(lastVersionPath);
    std::ifstream workingFile(fileName);

    if (!committedFile.is_open() || !workingFile.is_open()) {
        std::cerr << "Could not open files for comparison.\n";
        return;
    }

    std::string lineCommitted, lineWorking;
    int lineNum = 1;
    bool hasDiff = false;

    while (std::getline(committedFile, lineCommitted) || std::getline(workingFile, lineWorking)) {
        // Fill empty lines to keep line alignment
        if (committedFile.eof()) lineCommitted = "";
        if (workingFile.eof()) lineWorking = "";

        if (lineCommitted != lineWorking) {
            hasDiff = true;
            std::cout << "Line " << lineNum << ":\n";
            std::cout << "  - Committed: \"" << lineCommitted << "\"\n";
            std::cout << "  + Working:   \"" << lineWorking << "\"\n";
        }
        ++lineNum;
    }

    if (!hasDiff) {
        std::cout << "✅ No differences found. Working copy matches last commit.\n";
    }
}


// Helper function that Generates a dummy commit ID to differentiate our commits
std::string MiniGit::generateCommitID() {
    static int counter = 0;
    return "commit" + std::to_string(++counter);
}

