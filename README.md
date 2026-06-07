# MiniShell - Unix-like Shell in C

A minimal Unix-style shell implemented in C as a systems programming project.

This project demonstrates core operating system concepts such as process creation, program execution, inter-process communication, and file descriptor manipulation.

---

## Features (Current Version)

### Core Shell Features
- Command execution using `fork()` and `execvp()`
- Support for command-line arguments (e.g., `ls -l`)
- Basic command parsing using `strtok()`
- Parent process synchronization using `waitpid()`

### Pipeline Support
- Multi-stage pipelines using `|`
- Example:
  ```bash
  ls | grep c | wc -l
