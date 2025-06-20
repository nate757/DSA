# MiniGit – CLI-Based Version Control System

## 📌 Overview
MiniGit is a simplified version control system implemented in **C++** that simulates some of Git’s basic features. It allows users to initialize a repository, add files, commit changes, and track version history through a command-line interface.

---

## 🧱 Data Structures Used

- **Singly Linked List**  
  Used to represent the commit history. Each node holds metadata for a commit and links to the previous one.

- **Custom Struct for File Tracking**  
  Each file in a commit is stored with metadata like version number and file name.

---

## ⚙️ Design Decisions

- **CLI Menu System**  
  The system uses simple console input/output for interacting with the user. Commands include:
  - `init` – initialize MiniGit repo
  - `add` – stage files for commit
  - `commit` – save changes
  - `log` – show history
  - `checkout` – revert to previous commit

- **Internal File Handling**  
  All committed files are saved in a hidden `.minigit/` folder, with unique version names (e.g., `file_1.txt`).

- **No External Libraries**  
  Only C++ standard libraries are used for file I/O and data structure management.

---

## 🚫 Limitations & Future Improvements

- ❌ No support for branches or merges  
- ❌ No remote repository integration  
- ❌ File comparison is basic (can be improved using hashing)

### 🔮 Future Ideas:
- Add branching and merging features  
- Improve `diff` and conflict detection  
- Use SHA-based hashing for versioning  
- Replace CLI with graphical interface (GUI)
