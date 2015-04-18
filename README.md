# rshell
This is a command shell. Programmed in C++ and makes use of the system calls using execvp

##Introduction

This project is built to act as a terminal. It has the same functionality as a regular terminal, it is able to perform all the execvp functions and gives proper feed back on whether the functions have worked or failed.

##Install Instructions

1. git clone `https://github.com/divyanshch/rshell.git`
2. `cd rshell`
3. creat executables `make`
4. execute rshell `bin/rshell`

##Bugs/Limitations

1. The same connectors in a row will act as if they were just the actual connector 

	`|||||||||` is the same as `||`

	`&&&&&&&&&` is the same as `&&`

	`;;;;;;;;;` is the same as `;`

2. 	`ls ; && ls -a` works, but it only executes the first ls 

3. Having just `|` or `&` acts the same way as having `&&` or `||`

4. Cannot run `"ls"` (that is `ls` in quotes `""`) throws an error `execvp: No such file or directory` 



