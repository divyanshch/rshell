CFLAG = -ansi -pedantic -Wall -Werror

COMPILER = g++ 

all:
	mkdir -p bin
	$(COMPILER) $(CFLAG) src/main.cpp -o bin/rshell

rshell:
	mkdir -p bin
	$(COMPILER) $(CFLAG) src/main.cpp -o bin/rshell 
