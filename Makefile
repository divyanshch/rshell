CFLAG  = -ansi -pedantic -Wall -Werror
COMPILER = g++ 

all:
	mkdir -p bin

	$(COMPILER) $(CFLAG) src/main.cpp -o bin/rshell
	$(COMPILER) $(CFLAG) src/ls.cpp   -o bin/ls
	$(COMPILER) $(CFLAG) src/cp.cpp   -o bin/cp
	$(COMPILER) $(CFLAG) src/rm.cpp   -o bin/rm
	$(COMPILER) $(CFLAG) src/mv.cpp   -o bin/mv


rshell:
	mkdir -p bin
	$(COMPILER) $(CFLAG) src/main.cpp -o bin/rshell 

ls:
	mkdir -p bin
	$(COMPILER) $(CFLAG) src/ls.cpp   -o bin/ls 

rm:
	mkdir -p bin
	$(COMPILER) $(CFLAG) src/rm.cpp   -o bin/rm 

mv:
	mkdir -p bin
	$(COMPILER) $(CFLAG) src/mv.cpp   -o bin/mv 

cp:
	mkdir -p bin
	$(COMPILER) $(CFLAG) src/cp.cpp   -o bin/cp 
