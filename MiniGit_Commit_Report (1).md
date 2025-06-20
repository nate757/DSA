
## MiniGit Commit Functionality Report

**Author**: [Your Name]
**Role**: Commit Function – Group Member

---

## :pushpin: Core Purpose

The `commit()` function in MiniGit captures the project's tracked file state by:

- Creating immutable file version snapshots
- Maintaining historical records through linked lists
- Enabling a linear version history for the project

---

## :mag: Data Structures

### Commit History

| Component     | Description                                        |
| ------------- | -------------------------------------------------- |
| **Structure** | Singly linked list                                 |
| **Nodes**     | Store `commitID` and pointer to tracked files list |
| **Traversal** | Linear: points to the next commit node only        |

### File Version Tracking

| Component     | Description                                            |
| ------------- | ------------------------------------------------------ |
| **Structure** | Doubly linked list                                     |
| **Nodes**     | Hold `fileName` and `fileVersion` (e.g., `file_1.txt`) |
| **Links**     | Allow navigation across tracked files in each commit   |

---

## :hammer_and_wrench: Design Choices

| Decision                      | Rationale                                                                     |
| ----------------------------- | ----------------------------------------------------------------------------- |
| **Linked Lists**              | Dynamic and easy to expand as new commits/files are added                     |
| **Versioned File Naming**     | Avoids overwriting; each version gets a unique file name (e.g., `file_2.txt`) |
| **No user-supplied metadata** | Keeps system simple and educational, focusing on file tracking logic          |

**Commit Workflow:**

1. Files are added to tracking via `add()`.
2. `commit()` checks each file:
   - If changed, saves new version (e.g., `file_2.txt`).
   - Updates commit history accordingly.

---

## :warning: Limitations

| Type                | Details                                                                  |
| ------------------- | ------------------------------------------------------------------------ |
| **Functional**      | No commit messages or timestamps recorded.                               |
| **Structural**      | Linear history only — no support for branching/merging in this function. |
| **Redundancy Risk** | Possible creation of commits even if no file changes were made.          |

---

## :rocket: Future Improvements

| Priority   | Suggestion                                                  |
| ---------- | ----------------------------------------------------------- |
| **High**   | Add commit metadata (messages, timestamps)                  |
| **High**   | Implement SHA-1 or MD5 hashing for content change detection |
| **Medium** | Prevent unnecessary commits when no files have changed      |
| **Low**    | Integrate better with branching/merging logic               |

---

## :books: Implementation Context

| Aspect       | Detail                                                                                     |
| ------------ | ------------------------------------------------------------------------------------------ |
| **Location** | `MiniGit::commit()` in `MiniGit.cpp` file                                                  |
| **Purpose**  | To mimic basic Git commit behavior using C++ data structures for educational understanding |

---

### :white_check_mark: Summary

This design uses **singly and doubly linked lists** to maintain file versioning across commits, ensuring an expandable and traceable version history. While simple, it sets the foundation for more complex features like metadata, hashing, and branching in future improvements.
