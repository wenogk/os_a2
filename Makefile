CC=g++
all: chef saladMaker

chef: chef.cpp
	$(CC) chef.cpp -o chef -lpthread

saladMaker: saladMaker.cpp
	$(CC) saladMaker.cpp -o saladMaker -lpthread

clean:
	rm *.o chef saladMaker
