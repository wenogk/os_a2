
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
#include <sys/wait.h>

int tomatoWeight = 80;
int greenPepperWeight = 50;
int onionsWeight = 30;

// vegetable code 0 is for tomato
// vegetable code 1 is for greenPepper
// vegetable code 2 is for onions

//kitchen data structure for shared memory

int main(int argc, char *argv[])
{
    sem_t *Tomato_GreenPepper_semaphore;
    sem_t *Tomato_Onions_semaphore;
    sem_t *GreenPepper_Onions_semaphore;
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
    // shmid = shmget(IPC_PRIVATE, sizeof(struct ChefTable), 0644 | IPC_CREAT); //create shared memory segment
    // if (shmid == -1)
    // {
    //     perror("Shared memory");
    //     return 1;
    // }

    // kitchenData = (ChefTable *)shmat(shmid, NULL, 0); // Attach to the shared memory segment to get a pointer to it.

    // if (kitchenData == (void *)-1)
    // {
    //     perror("Shared memory attach");
    //     return 1;
    // }
    // else
    // {
    //     printf("shmid %d\n", shmid);
    // }
    // kitchenData->totalSaladsNeeded = atoi(n_opt);
    // kitchenData->totalSaladsServed = 0;
    // // chef
    // printf("I am a chef\n");
    // printf("\t tomatoWeight: %d\n", tomatoWeight);

    // 3 SaladMakers, therefore create 3 salad maker processes

    if ((Tomato_GreenPepper_semaphore = sem_open(vegetablePairEnumToSemaphoreName(Tomato_GreenPepper).c_str(), O_CREAT, 0666, 1)) == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }
    // if (*Tomato_GreenPepper_semaphore < 0)
    // {
    //     perror("Tomato_GreenPepper_semaphore opening error");
    //     return 1;
    // }
    if ((Tomato_Onions_semaphore = sem_open(vegetablePairEnumToSemaphoreName(Tomato_Onions).c_str(), O_CREAT, 0666, 1)) == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    // if (*Tomato_Onions_semaphore < 0)
    // {
    //     perror("Tomato_Onions_semaphore");
    //     return 1;
    // }

    if ((GreenPepper_Onions_semaphore = sem_open(vegetablePairEnumToSemaphoreName(GreenPepper_Onions).c_str(), O_CREAT, 0666, 1)) == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    int value;
    sem_getvalue(Tomato_GreenPepper_semaphore, &value);

    pid_t pids[3];

    for (int i = 0; i < 3; i++)
    {
        pids[i] = fork();
        if (pids[i] == 0)
        {
            char saladMakerNumberChar[2];
            sprintf(saladMakerNumberChar, "%d", i);
            char *sorterData[8] = {"./saladMaker", "-m", "5", "-s", "43342", "-n", saladMakerNumberChar, (char *)NULL};
            if (execv(sorterData[0], sorterData) == -1)
            {
                perror("Error creating salad maker process");
            }
        }
    }

    // if (*GreenPepper_Onions_semaphore < 0)
    // {
    //     perror("GreenPepper_Onions_semaphore");
    //     return 1;
    // }

    // while (the total number of salads needed - salads served) is > 0
    //----randomly select 1 pair of veggies
    //----randomly set weights for the two veggie variables

    //----V() the semaphore for that pair of veggies - aka giving the veggies to the salad maker to make

    //----P() the semaphore for that pair of veggies - aka waiting for salad maker to make
    sem_getvalue(Tomato_GreenPepper_semaphore, &value);
    printf("1) %s semaphore value is %d\n", vegetablePairEnumToSemaphoreName(Tomato_GreenPepper).c_str(), value);
    if (sem_post(Tomato_GreenPepper_semaphore) < 0)
    {
        perror("sem post");
        return 1;
    }
    sem_getvalue(Tomato_GreenPepper_semaphore, &value);
    printf("2) %s semaphore value is %d\n", vegetablePairEnumToSemaphoreName(Tomato_GreenPepper).c_str(), value);

    // if (sem_wait(Tomato_GreenPepper_semaphore) < 0)
    // {
    //     perror("sem wait");
    //     return 1;
    // }

    // sem_wait(Tomato_Onions_semaphore);

    // sem_post(Tomato_Onions_semaphore);

    // sem_wait(GreenPepper_Onions_semaphore);

    // sem_post(GreenPepper_Onions_semaphore);

    //kill all saladMakers :D

    for (int i = 0; i < 3; i++)
    {
        wait(NULL); // wait for each salad maker
    }

    sem_unlink(vegetablePairEnumToSemaphoreName(GreenPepper_Onions).c_str());
    sem_unlink(vegetablePairEnumToSemaphoreName(Tomato_GreenPepper).c_str());
    sem_unlink(vegetablePairEnumToSemaphoreName(Tomato_Onions).c_str());

    sem_close(Tomato_GreenPepper_semaphore);
    sem_close(Tomato_Onions_semaphore);
    sem_close(GreenPepper_Onions_semaphore);

    return 0;
}