
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include "kitchen.h"

//f
#include <fcntl.h>    /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <semaphore.h>

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
    me = getSaladMakerFromSaladMakerNumber(saladMakerNumber);

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

    sem_unlink(vegetablePairEnumToSemaphoreName(me->vegetablesNeeded).c_str());
    int value;
    sem_getvalue(mutex, &value);
    printf("%s value is %d\n", vegetablePairEnumToSemaphoreName(me->vegetablesNeeded).c_str(), value);
    printf("Salad maker before\n");

    if (sem_post(full) < 0)
    {
        perror("mutex probblem");
        return 1;
    }

    if (sem_post(mutex) < 0)
    {
        perror("mutex probblem");
        return 1;
    }

    //critical section

    printf("IN CRITICAL SECTION OF SALAD MAKER!!");

    sleep(3);

    if (sem_wait(mutex) < 0)
    {
        perror("mutex probblem");
        return 1;
    }

    if (sem_wait(empty) < 0)
    {
        perror("mutex probblem");
        return 1;
    }

    if (sem_post(done) < 0)
    {
        perror("mutex probblem");
        return 1;
    }

    sem_unlink(vegetablePairEnumToSemaphoreName_Empty(me->vegetablesNeeded).c_str());
    sem_unlink(vegetablePairEnumToSemaphoreName_Full(me->vegetablesNeeded).c_str());
    sem_unlink(vegetablePairEnumToSemaphoreName_Mutex(me->vegetablesNeeded).c_str());
    sem_unlink(vegetablePairEnumToSemaphoreName_Done(me->vegetablesNeeded).c_str());

    sem_close(empty);
    sem_close(full);
    sem_close(mutex);
    sem_close(done);

    delete me;
    return 0;
}