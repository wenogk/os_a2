
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
#include <cstdlib>
#include <signal.h>
#include <ctime>

int tomatoWeight = 80;
int greenPepperWeight = 50;
int onionsWeight = 30;

int randNum(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

int main(int argc, char *argv[])
{
    srand(time(0));
    sem_t *Tomato_GreenPepper_semaphore_empty;
    sem_t *Tomato_GreenPepper_semaphore_full;
    sem_t *Tomato_GreenPepper_semaphore_mutex;
    sem_t *Tomato_GreenPepper_semaphore_done;

    sem_t *Tomato_Onions_semaphore_empty;
    sem_t *Tomato_Onions_semaphore_full;
    sem_t *Tomato_Onions_semaphore_mutex;
    sem_t *Tomato_Onions_semaphore_done;

    sem_t *GreenPepper_Onions_semaphore_empty;
    sem_t *GreenPepper_Onions_semaphore_full;
    sem_t *GreenPepper_Onions_semaphore_mutex;
    sem_t *GreenPepper_Onions_semaphore_done;

    int opt;
    bool flagN = false; //number of salads flag
    bool flagM = false; //chhef time flag

    char *n_opt = 0;
    char *m_opt = 0;

    //get the arguments for the chef program
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
        //if -n and -m flag values not given, exit and show error msg:
        printf("Argument error. Chef should be invoked as follows: ./chef -n numOfSalads -m cheftime \n");
        return 1;
    }

    int shmid, numtimes;
    struct ChefBook *chefBook;
    char *bufptr;
    int spaceavailable;

    shmid = shmget(IPC_PRIVATE, sizeof(struct ChefBook), 0644 | IPC_CREAT); //create shared memory segment
    if (shmid == -1)
    {
        perror("Shared memory");
        return 1;
    }

    chefBook = (ChefBook *)shmat(shmid, NULL, 0); // Attach to the shared memory segment to get a pointer to it.

    if (chefBook == (void *)-1)
    {
        perror("Shared memory attach");
        return 1;
    }
    else
    {
        printf("shmid %d\n", shmid);
    }

    if ((Tomato_GreenPepper_semaphore_empty = sem_open(vegetablePairEnumToSemaphoreName_Empty(Tomato_GreenPepper).c_str(), O_CREAT, 0666, 1)) == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    if ((Tomato_GreenPepper_semaphore_full = sem_open(vegetablePairEnumToSemaphoreName_Full(Tomato_GreenPepper).c_str(), O_CREAT, 0666, 0)) == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    if ((Tomato_GreenPepper_semaphore_mutex = sem_open(vegetablePairEnumToSemaphoreName_Mutex(Tomato_GreenPepper).c_str(), O_CREAT, 0666, 1)) == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    if ((Tomato_GreenPepper_semaphore_done = sem_open(vegetablePairEnumToSemaphoreName_Done(Tomato_GreenPepper).c_str(), O_CREAT, 0666, 0)) == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    //Salad maker who wants onions and tomatoes semaphores

    if ((Tomato_Onions_semaphore_empty = sem_open(vegetablePairEnumToSemaphoreName_Empty(Tomato_Onions).c_str(), O_CREAT, 0666, 1)) == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    if ((Tomato_Onions_semaphore_full = sem_open(vegetablePairEnumToSemaphoreName_Full(Tomato_Onions).c_str(), O_CREAT, 0666, 0)) == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    if ((Tomato_Onions_semaphore_mutex = sem_open(vegetablePairEnumToSemaphoreName_Mutex(Tomato_Onions).c_str(), O_CREAT, 0666, 1)) == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    if ((Tomato_Onions_semaphore_done = sem_open(vegetablePairEnumToSemaphoreName_Done(Tomato_Onions).c_str(), O_CREAT, 0666, 0)) == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    //Salad maker who wants green pepper and onions semaphores

    if ((GreenPepper_Onions_semaphore_empty = sem_open(vegetablePairEnumToSemaphoreName_Empty(GreenPepper_Onions).c_str(), O_CREAT, 0666, 1)) == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    if ((GreenPepper_Onions_semaphore_full = sem_open(vegetablePairEnumToSemaphoreName_Full(GreenPepper_Onions).c_str(), O_CREAT, 0666, 0)) == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    if ((GreenPepper_Onions_semaphore_mutex = sem_open(vegetablePairEnumToSemaphoreName_Mutex(GreenPepper_Onions).c_str(), O_CREAT, 0666, 1)) == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    if ((GreenPepper_Onions_semaphore_done = sem_open(vegetablePairEnumToSemaphoreName_Done(GreenPepper_Onions).c_str(), O_CREAT, 0666, 0)) == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    pid_t timeLoggerPid;
    timeLoggerPid = fork();
    if (timeLoggerPid == 0)
    {
        char shmidChar[15];
        sprintf(shmidChar, "%d", shmid);
        char *sorterData[4] = {"./timeLogger", "-s", shmidChar, (char *)NULL};
        if (execv(sorterData[0], sorterData) == -1)
        {
            perror("Error creating salad maker process");
        }
    }

    // 3 SaladMakers, therefore create 3 salad maker processes and send shared memory shmid and salad maker index
    pid_t pids[3];

    for (int i = 0; i < 3; i++)
    {
        pids[i] = fork();
        if (pids[i] == 0)
        {
            char saladMakerNumberChar[2];
            sprintf(saladMakerNumberChar, "%d", i);
            char shmidChar[15];
            sprintf(shmidChar, "%d", shmid);
            char *sorterData[8] = {"./saladMaker", "-m", "5", "-s", shmidChar, "-n", saladMakerNumberChar, (char *)NULL};
            if (execv(sorterData[0], sorterData) == -1)
            {
                perror("Error creating salad maker process");
            }
        }
    }

    // while (the total number of salads needed - salads served) is > 0
    //----randomly select 1 pair of veggies
    //----randomly set weights for the two veggie variables

    int totalSaladsNeeded = atoi(n_opt);
    int totalVeggiePairsGivenToSaladMakers = 0;

    while (totalVeggiePairsGivenToSaladMakers < totalSaladsNeeded)
    {
        int chosenSaladMakerIndex = randNum(0, 2);
        printf("Randomly chose %d\n", chosenSaladMakerIndex);
        ////sleep(3);
        if (chosenSaladMakerIndex == 0)
        {
            if (sem_wait(GreenPepper_Onions_semaphore_empty) < 0)
            {
                perror("sem wait");
                return 1;
            }

            if (sem_wait(GreenPepper_Onions_semaphore_mutex) < 0)
            {
                perror("sem wait");
                return 1;
            }

            ////sleep(2);

            if (sem_post(GreenPepper_Onions_semaphore_mutex) < 0)
            {
                perror("sem post");
                return 1;
            }

            if (sem_post(GreenPepper_Onions_semaphore_full) < 0)
            {
                perror("sem post");
                return 1;
            }

            printf("CHEF waiting for Salad maker now.. \n");

            if (sem_wait(GreenPepper_Onions_semaphore_done) < 0)
            {
                perror("sem wait");
                return 1;
            }
        }
        else if (chosenSaladMakerIndex == 1)
        {
            if (sem_wait(Tomato_Onions_semaphore_empty) < 0)
            {
                perror("sem wait");
                return 1;
            }

            if (sem_wait(Tomato_Onions_semaphore_mutex) < 0)
            {
                perror("sem wait");
                return 1;
            }

            printf("Giving ingredients to salad maker! \n");

            ////sleep(2);

            if (sem_post(Tomato_Onions_semaphore_mutex) < 0)
            {
                perror("sem post");
                return 1;
            }

            if (sem_post(Tomato_Onions_semaphore_full) < 0)
            {
                perror("sem post");
                return 1;
            }

            printf("CHEF waiting for Salad maker now.. \n");

            if (sem_wait(Tomato_Onions_semaphore_done) < 0)
            {
                perror("sem wait");
                return 1;
            }
        }
        else if (chosenSaladMakerIndex == 2)
        {
            if (sem_wait(Tomato_GreenPepper_semaphore_empty) < 0)
            {
                perror("sem wait");
                return 1;
            }

            if (sem_wait(Tomato_GreenPepper_semaphore_mutex) < 0)
            {
                perror("sem wait");
                return 1;
            }

            printf("Giving ingredients to salad maker! \n");

            ////sleep(3);

            if (sem_post(Tomato_GreenPepper_semaphore_mutex) < 0)
            {
                perror("sem post");
                return 1;
            }

            if (sem_post(Tomato_GreenPepper_semaphore_full) < 0)
            {
                perror("sem post");
                return 1;
            }

            printf("CHEF waiting for Salad maker to pick up veggies now.. \n");
            //sleep(3);

            if (sem_wait(Tomato_GreenPepper_semaphore_done) < 0)
            {
                perror("sem wait");
                return 1;
            }
        }

        printf("Serving salad.. \n");
        totalVeggiePairsGivenToSaladMakers += 1;
    }

    while (chefBook->isSaladMakerDoingWork[0] || chefBook->isSaladMakerDoingWork[1] || chefBook->isSaladMakerDoingWork[2])
        ;

    logChefBook(chefBook);

    for (int i = 0; i < 3; i++)
    {
        kill(pids[i], SIGTERM); // kill each salad maker
    }

    kill(timeLoggerPid, SIGTERM);

    sem_unlink(vegetablePairEnumToSemaphoreName_Empty(Tomato_GreenPepper).c_str());
    sem_unlink(vegetablePairEnumToSemaphoreName_Full(Tomato_GreenPepper).c_str());
    sem_unlink(vegetablePairEnumToSemaphoreName_Mutex(Tomato_GreenPepper).c_str());
    sem_unlink(vegetablePairEnumToSemaphoreName_Done(Tomato_GreenPepper).c_str());

    sem_unlink(vegetablePairEnumToSemaphoreName_Empty(Tomato_Onions).c_str());
    sem_unlink(vegetablePairEnumToSemaphoreName_Full(Tomato_Onions).c_str());
    sem_unlink(vegetablePairEnumToSemaphoreName_Mutex(Tomato_Onions).c_str());
    sem_unlink(vegetablePairEnumToSemaphoreName_Done(Tomato_Onions).c_str());

    sem_unlink(vegetablePairEnumToSemaphoreName_Empty(GreenPepper_Onions).c_str());
    sem_unlink(vegetablePairEnumToSemaphoreName_Full(GreenPepper_Onions).c_str());
    sem_unlink(vegetablePairEnumToSemaphoreName_Mutex(GreenPepper_Onions).c_str());
    sem_unlink(vegetablePairEnumToSemaphoreName_Done(GreenPepper_Onions).c_str());

    sem_close(Tomato_GreenPepper_semaphore_empty);
    sem_close(Tomato_GreenPepper_semaphore_full);
    sem_close(Tomato_GreenPepper_semaphore_mutex);
    sem_close(Tomato_GreenPepper_semaphore_done);

    sem_close(Tomato_Onions_semaphore_empty);
    sem_close(Tomato_Onions_semaphore_full);
    sem_close(Tomato_Onions_semaphore_mutex);
    sem_close(Tomato_Onions_semaphore_done);

    sem_close(GreenPepper_Onions_semaphore_empty);
    sem_close(GreenPepper_Onions_semaphore_full);
    sem_close(GreenPepper_Onions_semaphore_mutex);
    sem_close(GreenPepper_Onions_semaphore_done);

    shmdt(chefBook);               //detatch from the shared memory segment
    shmctl(shmid, IPC_RMID, NULL); //delete the shared memory segment

    return 0;
}