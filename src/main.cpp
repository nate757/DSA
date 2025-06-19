#include "MiniGit.hpp"
#include <iostream>
#include <sstream>

int main() {
    MiniGit git;
    std::string input;

    std::cout << "Welcome to MiniGit CLI ✨\n";
    std::cout << "Type 'help' to view available commands.\n";

    while (true) {
        std::cout << "\nMiniGit> ";
        std::getline(std::cin, input);
        std::istringstream iss(input);
        std::string command;
        iss >> command;

        if (command == "init") {
            git.init();
        } else if (command == "add") {
            std::string fileName;
            iss >> fileName;
            git.add(fileName);
        } else if (command == "commit") {
            std::string message;
            std::getline(iss >> std::ws, message);
            git.commit(message);
        } else if (command == "log") {
            git.log();
        } else if (command == "checkout") {
            std::string id;
            iss >> id;
            git.checkout(id);
        } else if (command == "branch") {
            std::string name;
            iss >> name;
            git.branch(name);
        } else if (command == "checkout-branch") {
            std::string name;
            iss >> name;
            git.checkoutBranch(name);
        } else if (command == "merge") {
            std::string name;
            iss >> name;
            git.merge(name);
        } else if (command == "help") {
            std::cout << "\nAvailable commands:\n";
            std::cout << "  init                  - Initialize repository\n";
            std::cout << "  add <filename>        - Stage a file\n";
            std::cout << "  commit <message>      - Commit staged changes\n";
            std::cout << "  log                   - View commit history\n";
            std::cout << "  checkout <commitID>   - Restore files from commit\n";
            std::cout << "  branch <name>         - Create a new branch\n";
            std::cout << "  checkout-branch <name> - Switch to a branch\n";
            std::cout << "  merge <branch>        - Merge a branch into current\n";
            std::cout << "  exit                  - Exit MiniGit\n";
        } else if (command == "exit") {
            std::cout << "Exiting MiniGit. Keep committing greatness! 💾\n";
            break;
        } else {
            std::cout << "Unrecognized command. Type 'help' to see what's available.\n";
        }
    }

    return 0;
}


// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.