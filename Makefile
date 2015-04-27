COMPILER = g++
CFLAGS = -std=c++0x -pedantic -Wall -Werror
RESULT = ./bin/rshell
DIRECTORY = ./src/main.cpp
all:
	mkdir -p ./bin
	$(COMPILER) $(CFLAGS) $(DIRECTORY) -o $(RESULT)
	$(COMPILER) $(CFLAGS) ./src/cp.cpp -o ./bin/cp

rshell:
	mkdir -p ./bin
	$(COMPILER) $(CFLAGS) $(DIRECTORY) -o $(RESULT)
cp:
	mkdir -p ./bin
	$(COMPILER) $(CFLAGS) ./src/cp.cpp -o ./bin/cp
clean:
	rm -rf ./bin
