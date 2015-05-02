CFLAG = -ansi -pedantic -Wall -Werror

COMPILER = g++ 

all:
	mkdir -p bin

	$(COMPILER) $(CFLAG) src/main.cpp -o bin/rshell
	$(COMPILER) $(CFLAG) src/ls.cpp   -o bin/ls
	$(COMPILER) $(CFLAG) src/cp.cpp   -o bin/cp
rshell:
	mkdir -p bin
	$(COMPILER) $(CFLAG) src/main.cpp -o bin/rshell 

ls:
	mkdir -p bin
	$(COMPILER) $(CFLAG) src/ls.cpp   -o bin/ls 

cp:
	mkdir -p bin
	$(COMPILER) $(CFLAG) src/cp.cpp   -o bin/cp 
