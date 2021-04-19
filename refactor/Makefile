CC=gcc
all: chef saladMaker

chef: chef.c
	$(CC) chef.c -o chef -lpthread

saladMaker: saladMaker.c
	$(CC) saladMaker.c -o saladMaker -lpthread

clean:
	rm *.o chef saladMaker
