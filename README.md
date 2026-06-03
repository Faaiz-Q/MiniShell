# MyShell - Unix-like Shell in C

A minimal Unix-style shell implemented in C as a systems programming project.

This project demonstrates core operating system concepts such as process creation, program execution, inter-process communication, and file descriptor manipulation.

---

## Features (Current Version)

- Command execution using `fork()` and `execvp()`
- Support for arguments (e.g., `ls -l`)
- Input parsing using `strtok()`
- Basic pipeline support using `|`
  - Example: `ls | grep c`
- Parent process synchronization using `wait()`

---

## Concepts Learned

- Process creation (`fork`)
- Process replacement (`execvp`)
- Process synchronization (`wait`)
- File descriptors
- Pipes (`pipe()`)
- I/O redirection (`dup2`)
- Basic command parsing

---

## Example Usage

```bash
myshell> ls
myshell> ls -l
myshell> ls | grep c
myshell> pwd
myshell> exit
