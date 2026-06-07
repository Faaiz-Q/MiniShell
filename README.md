# MyShell - Unix-like Shell in C

A minimal Unix-style shell implemented in C as a systems programming project.

This project demonstrates core operating system concepts such as process creation, program execution, inter-process communication, and file descriptor manipulation.

---

## Features (Current Version)

### Core Shell Features
- Command execution using `fork()` and `execvp()`
- Support for command-line arguments (e.g., `ls -l`)
- Basic command parsing using `strtok()`
- Parent process synchronization using `waitpid()`

---

### Pipeline Support (`|`)
Supports multi-stage pipelines.

Example:
```bash
ls | grep c | wc -l

Implementation:

pipe() for communication between processes
dup2() to connect stdout of one process to stdin of the next
Input Redirection (<)

Redirects standard input from a file instead of keyboard input.

Example:

sort < input.txt
cat < file.txt

Implementation:

open() with O_RDONLY
dup2(fd, STDIN_FILENO)
Output Redirection (>)

Redirects standard output to a file (overwrites file if it exists).

Example:

ls > output.txt
echo hello > file.txt

Implementation:

open() with O_WRONLY | O_CREAT | O_TRUNC
dup2(fd, STDOUT_FILENO)
Combined Usage (Pipes + Redirection)

The shell supports mixing pipes with input/output redirection.

Examples:

cat < input.txt | grep hello > output.txt
ls | wc -l > count.txt
sort < input.txt | grep error | wc -l > result.txt

Behavior:

Pipes connect intermediate commands
< applies to the first command only
> applies to the last command only
dup2() is used before execvp() to modify file descriptors per process
Concepts Learned
Process creation using fork()
Program execution using execvp()
Process synchronization using waitpid()
File descriptor manipulation
Inter-process communication using pipes (pipe())
Input/output redirection using dup2()
File handling using open()
Unix execution model:
fork → setup file descriptors → execvp → wait
Supported Example Commands
myshell> ls
myshell> ls -l
myshell> pwd
myshell> ls | grep c
myshell> ls | wc -l
myshell> sort < input.txt
myshell> echo hello > out.txt
myshell> cat < input.txt | grep hello > result.txt
myshell> ls | wc > count.txt
myshell> sort < input.txt | grep error | wc -l > result.txt
myshell> exit
Limitations
No support for quotes (e.g., "hello world")
No append redirection (>>)
No background execution (&)
Simple parsing using whitespace (strtok), so spacing is required around | < >
No built-in commands like cd, pwd
Future Improvements
Add built-in commands (cd, pwd)
Support background processes (&)
Add append redirection (>>)
Improve parsing (support quotes and escape characters)
Add signal handling (Ctrl+C, zombie process cleanup)
Replace strtok with a more robust tokenizer
Summary

This project implements the core Unix shell execution model:

parse → fork → setup pipes/redirection → execvp → wait

It demonstrates how Unix-like shells internally manage processes and file descriptors to support pipelines and I/O redirection.
