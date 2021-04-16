CC=g++
all: chef saladMaker

main: chef.cpp
	$(CC) -w chef.cpp -o chef

saladMaker: saladMaker.cpp
	$(CC) -w saladMaker.cpp -o saladMaker

clean:
	rm *.o chef saladMaker
