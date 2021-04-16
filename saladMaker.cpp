
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include "kitchen.h"

int main(int argc, char *argv[])
{
    sem_t *mutex;
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

    printf("saladMaker mutex---> %s \n", vegetablePairEnumToString(me->vegetablesNeeded).c_str());
    mutex = sem_open(vegetablePairEnumToString(me->vegetablesNeeded).c_str(), O_CREAT, 0666, 0);

    sem_wait(mutex);
    sleep(3);
    printf("I am a saladmaker\n");
    if (me->vegetableInfiniteSupply == Tomato)
    {
        printf("\t I have unlimited tomatoes \n");
    }

    if (me->vegetableInfiniteSupply == GreenPepper)
    {
        printf("\t I have unlimited green pepper \n");
    }

    if (me->vegetableInfiniteSupply == Onions)
    {
        printf("\t I have unlimited onions \n");
    }

    sem_post(mutex);
    sem_close(mutex);
    //while true
    //---start timer for waiting timer
    //---P(me->vegetablesNeeded)
    // open up shared memz
    //---end timer for waiting timer. add time waited to shared total waiting time for specific salad maker
    //---start timer for time making salad

    //---sleep based on random time

    //---end timer for time making. add time waited to shared total making time for specific salad maker
    //---add 1 to shared total number of salads made
    //---add 1 to shared total numbber of salads made for specific salad maker
    //close shared memz
    //---V(me->vegetablesNeeded)

    delete me;
    return 0;
}