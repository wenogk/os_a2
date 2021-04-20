CC=g++
all: chef saladMaker timeLogger

chef: chef.cpp
	$(CC) chef.cpp -o chef -lpthread

saladMaker: saladMaker.cpp
	$(CC) saladMaker.cpp -o saladMaker -lpthread

timeLogger : timeLogger.cpp
	$(CC) timeLogger.cpp -o timeLogger -lpthread

clean:
	rm *.o chef saladMaker timeLogger
