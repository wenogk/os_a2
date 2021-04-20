
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

#include <time.h>

int randNum(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

double randDouble(int min, int max)
{
    return (max - min) * ((double)rand() / (double)RAND_MAX) + min;
}

int main(int argc, char *argv[])
{
    sem_t *empty;
    sem_t *full;
    sem_t *mutex;
    sem_t *done;

    int opt;
    bool flagS = false; // identifier that the shared memory segments has
    bool flagM = false; //salad making time
    bool flagN = false; // saladMaker number (either 0, 1 or 2)

    char *s_opt = 0;
    char *m_opt = 0;
    char *n_opt = 0;
    while ((opt = getopt(argc, argv, "s:m:n:")) != -1)
    { // for each option... n and m
        switch (opt)
        {
        case 's':
            s_opt = optarg;
            flagS = true;
            break;
        case 'm':
            m_opt = optarg;
            flagM = true;
            break;
        case 'n':
            n_opt = optarg;
            flagN = true;
            break;
        case '?': // unknown option...
            printf("Unknown Option %s\n", opt);
            break;
        }
    }

    if (!flagS || !flagM || !flagN)
    {
        printf("Argument error. SaladMaker should be invoked as follows: ./saladmaker -m salmkrtime -s shmid -n saladMakerNumber \n");
        return 1;
    }

    SaladMaker *me;
    int saladMakerNumber = atoi(n_opt);
    int shmid = atoi(s_opt);
    double salmkrtime = atof(m_opt);
    me = getSaladMakerFromSaladMakerNumber(saladMakerNumber);

    struct ChefBook *chefBook = (ChefBook *)shmat(shmid, NULL, 0); /* Attach the segment */

    if (chefBook == (void *)-1)
    {
        perror("Shared memory attach");
        return 1;
    }
    else
    {
        printf("SaladMaker %d shmid %d\n", saladMakerNumber, shmid);
    }

    if ((empty = sem_open(vegetablePairEnumToSemaphoreName_Empty(me->vegetablesNeeded).c_str(), O_CREAT, 0666, 0)) == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    if ((full = sem_open(vegetablePairEnumToSemaphoreName_Full(me->vegetablesNeeded).c_str(), O_CREAT, 0666, 0)) == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    if ((mutex = sem_open(vegetablePairEnumToSemaphoreName_Mutex(me->vegetablesNeeded).c_str(), O_CREAT, 0666, 0)) == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    if ((done = sem_open(vegetablePairEnumToSemaphoreName_Done(me->vegetablesNeeded).c_str(), O_CREAT, 0666, 0)) == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    printf("Salad maker before\n");
    while (1)
    {
        time_t startWaiting = time(0); //time(0);
        printf("Waiting for ingredients %s clock is %f \n", vegetablePairEnumToSemaphoreName_Done(me->vegetablesNeeded).c_str(), float(startWaiting));
        //sleep(3);
        if (sem_wait(full) < 0)
        {
            perror("full probblem");
            return 1;
        }

        if (sem_wait(mutex) < 0)
        {
            perror("mutex probblem");
            return 1;
        }

        chefBook->isSaladMakerDoingWork[saladMakerNumber] = true;

        if (sem_post(done) < 0)
        {
            perror("mutex probblem");
            return 1;
        }
        printf("SALAD MAKER %d PICKED UP VEGGIES!!\n", saladMakerNumber);
        time_t endWaiting = time(0);
        double secondsSinceStartedToWait = difftime(endWaiting, startWaiting); //difftime(time(0), startWaiting);
        chefBook->SaladMakerTotalTimeWaiting[saladMakerNumber] += secondsSinceStartedToWait;

        printf("SALAD MAKER %d MAKING SALAD!!\n", saladMakerNumber);
        double saladMakingTime = randDouble(salmkrtime * 0.8, salmkrtime);
        sleep(saladMakingTime);
        chefBook->SaladMakerTotalTimeWorking[saladMakerNumber] += saladMakingTime;

        chefBook->NumberOfTotalSaladsMadeBySaladMaker[saladMakerNumber] += 1;

        chefBook->isSaladMakerDoingWork[saladMakerNumber] = false;

        printf("SALAD MAKER %d DONE MAKING SALAD!!\n", saladMakerNumber);

        if (sem_post(mutex) < 0)
        {
            perror("mutex probblem");
            return 1;
        }

        if (sem_post(empty) < 0)
        {
            perror("mutex probblem");
            return 1;
        }
    }

    sem_unlink(vegetablePairEnumToSemaphoreName_Empty(me->vegetablesNeeded).c_str());
    sem_unlink(vegetablePairEnumToSemaphoreName_Full(me->vegetablesNeeded).c_str());
    sem_unlink(vegetablePairEnumToSemaphoreName_Mutex(me->vegetablesNeeded).c_str());
    sem_unlink(vegetablePairEnumToSemaphoreName_Done(me->vegetablesNeeded).c_str());

    sem_close(empty);
    sem_close(full);
    sem_close(mutex);
    sem_close(done);

    int detatch = shmdt(chefBook); /* Detach segment */

    if (detatch < 0)
    {
        perror(" Detachment .\n");
        return 1;
    }

    delete me;
    return 0;
}