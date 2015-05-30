# rshell
This is a command shell. Programmed in C++ and makes use of the system calls using execvp.

##Introduction

This project is built to act as a terminal. It has the same functionality as a regular terminal, it is able to perform all the execvp functions and gives proper feedback on whether the functions have worked or failed.

##Install Instructions

```
$ clone  https://github.com/divyanshch/rshell.git
$ cd rshell
$ git checkout hw3
$ make
$ bin/rshell
```
##Bugs/Limitations for signaling

1. Does not handle the combination of connectors and `cd`

  ``echo hello && cd ..``

  This would result in the output of `hello` followed by an error.

2. If the path when running `cd` does not exist, there is an error message.

  ``cd non_existent_path``

  Output:

  ``chdir: No such file or directory``

3. If there is a second paramater passed into `cd` there will be a error.
  
  ``cd .. hello``

    Output:

    ``Error: invalid call of the cd command``





