# rshell
This is a command shell. Programmed in C++ and makes use of the system calls using execvp.

##Introduction

This project is built to act as a terminal. It has the same functionality as a regular terminal, it is able to perform all the execvp functions and gives proper feedback on whether the functions have worked or failed.

##Install Instructions

```
$ clone  https://github.com/divyanshch/rshell.git
$ cd rshell
$ git checkout hw2
$ make
$ bin/rshell
```
##Bugs/Limitations for io redirection and piping

1. Cannot handle differnet input, output redirections in the same line.

  `cat < file1 > file2`

  This results in a error message
  
  `Error: case not handled (cannot combine < and >)`

2. Cannot handle piping at all





