//named semaphore example
//compile with -lpthread : gcc process2.c -o process2 -lpthread

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>

int main() {

    sem_t * mutex;

    char * c = "This is the operating system course.\n";

    setbuf(stderr, NULL);  

    //already existing semaphore with same name available at this point
    mutex = sem_open("mutex_for_stderr", O_CREAT, 0666, 1);

    //it is blocked now because the value is 0 
    sem_wait(mutex);

    while (* c != '\0') {
        fputc(* c, stderr);
        c++;
        sleep(rand()%2+1);
    }

    sem_post(mutex);
    sem_close(mutex);
    sem_unlink("mutex_for_stderr");
    
    return 0;
}