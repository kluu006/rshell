COMPILER = g++
CFLAGS = -ansi -pedantic -Wall -Werror
RESULT = ./bin/rshell
DIRECTORY = ./src/main.cpp
all: 
	mkdir -p ./bin
	$(COMPILER) $(CFLAGS) $(DIRECTORY) -o $(RESULT)

rshell: 
	mkdir -p ./bin
	$(COMPILER) $(CFLAGS) $(DIRECTORY) -o $(RESULT)  
clean:
	rm -rf ./bin
