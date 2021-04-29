CC=g++
all: chef saladMaker timeLogger

chef: chef.cpp
	$(CC) chef.cpp -o chef -lpthread -w

saladMaker: saladMaker.cpp
	$(CC) saladMaker.cpp -o saladMaker -lpthread -w

timeLogger : timeLogger.cpp
	$(CC) timeLogger.cpp -o timeLogger -lpthread -w

clean:
	rm *.o chef saladMaker timeLogger
