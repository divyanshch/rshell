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

2. Having the same connectors followed by each other without any argument gives off a segmentation fault

	`ls || && ls -a` this works however its only because the first paramater is true hence it does not check the second paramater
	
	`asd || && ls -a` this on the other hand throws off an error
	
	`ls && || ls -a` also throws an error 
	
	`ls ; && ls` also throws an error
	
	Any combination of connectors right next to each other in that manner thows an error.

3. Having just `|` or `&` acts the same way as having `&&` or `||`

4. Cannot run `"ls"` throws an error `execvp: No such file or directory` 







