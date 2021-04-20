
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include "kitchen.h"
#include <ctime>

//f
#include <fcntl.h>    /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <semaphore.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[])
{

    int opt;
    bool flagS = false; // identifier that the shared memory segments has

    char *s_opt = 0;
    while ((opt = getopt(argc, argv, "s:")) != -1)
    { // for each option... n and m
        switch (opt)
        {
        case 's':
            s_opt = optarg;
            flagS = true;
            break;
        case '?': // unknown option...
            printf("Unknown Option %s\n", opt);
            break;
        }
    }

    if (!flagS)
    {
        printf("Argument error. timeLogger should be invoked as follows: ./timeLogger -s shmid \n");
        return 1;
    }

    int shmid = atoi(s_opt);

    struct ChefBook *chefBook = (ChefBook *)shmat(shmid, NULL, 0); /* Attach the segment */

    if (chefBook == (void *)-1)
    {
        perror("Shared memory attach");
        return 1;
    }
    else
    {
        printf("timeLogger %d shmid\n", shmid);
    }

    int detatch = shmdt(chefBook); /* Detach segment */

    if (detatch < 0)
    {
        perror(" Detachment .\n");
        return 1;
    }

    return 0;
}