
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
using namespace std;
#include "kitchen.h"
int tomatoWeight = 80;
int greenPepperWeight = 50;
int onionsWeight = 30;

// vegetable code 0 is for tomato
// vegetable code 1 is for greenPepper
// vegetable code 2 is for onions

//kitchen data structure for shared memory

int main(int argc, char *argv[])
{
    int opt;
    bool flagN = false; //number of salads flag
    bool flagM = false; //chhef time flag

    char *n_opt = 0;
    char *m_opt = 0;
    while ((opt = getopt(argc, argv, "n:m:")) != -1)
    { // for each option... n and m
        switch (opt)
        {
        case 'n':
            n_opt = optarg;
            flagN = true;
            break;
        case 'm':
            m_opt = optarg;
            flagM = true;
            break;
        case '?': // unknown option...
            printf("Unknown Option %s", opt);
            break;
        }
    }

    if (!flagN || !flagM)
    {
        printf("Argument error. Chef should be invoked as follows: ./chef -n numOfSalads -m cheftime \n");
        return 1;
    }

    int shmid, numtimes;
    struct ChefTable *kitchenData;
    char *bufptr;
    int spaceavailable;
    shmid = shmget(IPC_PRIVATE, sizeof(struct ChefTable), 0644 | IPC_CREAT); //create shared memory segment
    if (shmid == -1)
    {
        perror("Shared memory");
        return 1;
    }

    kitchenData = (ChefTable *)shmat(shmid, NULL, 0); // Attach to the shared memory segment to get a pointer to it.

    if (kitchenData == (void *)-1)
    {
        perror("Shared memory attach");
        return 1;
    }
    else
    {
        printf("shmid %d\n", shmid);
    }
    kitchenData->totalSaladsNeeded = atoi(n_opt);
    // chef
    printf("I am a chef\n");
    printf("\t tomatoWeight: %d", tomatoWeight);

    // 3 SaladMakers, therefore create 3 salad maker processes
    pid_t pids[3];

    for (int i = 0; i < 3; i++)
    {
        pids[i] = fork();
        if (pids[i] == 0)
        {
            char *sorterData[6] = {"./saladMaker", "-m", "5", "-s", "43342", (char *)NULL};
            if (execv(sorterData[0], sorterData) == -1)
            {
                perror("Error creating salad maker process");
            }
        }
    }

    for (int i = 0; i < 3; i++)
    {
        wait(NULL); // wait for each salad maker
    }

    return 0;
}