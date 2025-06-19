// src/MiniGit.hpp
#ifndef MINIGIT_HPP
#define MINIGIT_HPP

#include <string>
#include <filesystem>
#include <unordered_map>
#include <vector>

class MiniGit {
public:
    MiniGit();               // Constructor
    ~MiniGit();              // Destructor

    void init();             // Initializes the repo
    void checkout(const std::string& commitID);    //checks out a specific commit
    void merge(const std::string& branchName);    //merges a branch

private:
    std::string repoDir;     // Name of the .minigit directory
    struct Commit {
        std::string id;  //unique ID for commit
        std::unordered_map<std::string, std::string> fileSnapshots;  //mash filename->hash/version for each file in this commit
        std::string parnetID;   //ID for parent commit for history traversal
};

std::unordered_map<std::string, Commit> commitMap;  //maps commit ID to current commit (HEAD)
std::unordered_map<std::string, std::string> branches;  //maps branch name to the latest commit ID on that branch
std::string currentCommitID;
std::string currentBranch;

//internal helpers
std::string generateCommitID();
void restoreFiles(const Commit& commit);
void resolveConflicts(const Commit& current, const Commit& other);
#endif
