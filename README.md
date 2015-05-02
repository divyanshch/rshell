# rshell
This is a command shell. Programmed in C++ and makes use of the system calls using execvp.

##Introduction

This project is built to act as a terminal. It has the same functionality as a regular terminal, it is able to perform all the execvp functions and gives proper feedback on whether the functions have worked or failed.

##Install Instructions

```
$ clone  https://github.com/divyanshch/rshell.git
$ cd rshell
$ git checkout hw1
$ make
$ bin/ls
```
##Bugs/Limitations for ls

1. Can not handle any paramaters entered after the `~` is entered 

2. When `~` is used as a argument the output is in blue regardless of whether it is a directory or not.





