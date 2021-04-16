//named semaphore example
/*We use semaphore to ensure mutual exclusion.*/
//compile with -lpthread : gcc process1.c -o process1 -lpthread

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

    //initial value 1 set to semaphore
    mutex = sem_open("mutex_for_stderr", O_CREAT, 0666, 1);
    

    //suspend caller until semaphore's value > 0
    //if semaphore's value > 0, resource is AVAILABLE & decrement value by 1 
    //if semaphore current value == 0 , call BLOCKS (process waits) till value > 0
    
    sem_wait(mutex);
    //semaphore's value was == 1 so > 0 , decrement by 1, so value is now 0


    while (* c != '\0') {
      fputc(* c, stderr);
      c++;
      sleep(1);
    } 

    //increment semaphore value by 1 (it was 0, now it is 1) + 
    //(since value > 0 now) wake up any blocked processes waiting on the semaphore
    sem_post(mutex);

    //close semaphore
    sem_close(mutex);

    //unlink 
    sem_unlink("mutex_for_stderr");

    return 0;
}