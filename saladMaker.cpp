
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
#include <fstream>

//default weights of the veggies
double tomatoWeight = 80.0;
double greenPepperWeight = 50.0;
double onionsWeight = 30.0;

void logString(string logString)
{
    ofstream logFile;
    logFile.open("logFile.txt", ios::app);
    time_t currentTime = time(NULL);
    char *currentTime_no_newline;
    currentTime_no_newline = strdup(ctime(&currentTime)); //myctime[ strlen(myctime) - 1 ] = '\0';
    currentTime_no_newline[strlen(currentTime_no_newline) - 1] = '\0';
    std::string stringToAppend;
    stringToAppend += currentTime_no_newline;
    logFile << stringToAppend << " > " << logString << endl
            << "--------------" << endl;
    logFile.close();
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
        //printf("Waiting for ingredients %s clock is %f \n", vegetablePairEnumToSemaphoreName_Done(me->vegetablesNeeded).c_str(), float(startWaiting));

        logString("Salad Maker " + to_string(saladMakerNumber) + " waiting for " + vegetablePairEnumToNormalString(me->vegetablesNeeded).c_str());

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
        //printf("SALAD MAKER %d PICKED UP VEGGIES!!\n", saladMakerNumber);
        logString("Salad Maker " + to_string(saladMakerNumber) + " picked up " + vegetablePairEnumToNormalStringWithWeights(me->vegetablesNeeded, chefBook).c_str());
        time_t endWaiting = time(0);
        double secondsSinceStartedToWait = difftime(endWaiting, startWaiting); //difftime(time(0), startWaiting);
        chefBook->SaladMakerTotalTimeWaiting[saladMakerNumber] += secondsSinceStartedToWait;

        if (saladMakerNumber == 0)
        {
            double selfPickedTomatoWeight = randDouble(0.8 * tomatoWeight, 1.2 * tomatoWeight);
            chefBook->totalPickedOnionWeightForSaladMaker0 += chefBook->currentPickedOnionWeight;
            chefBook->totalPickedGreenPepperWeightForSaladMaker0 += chefBook->currentPickedGreenPepperWeight;
            chefBook->totalSelfPickedTomatoWeightForSaladMaker0 += selfPickedTomatoWeight;
        }
        else if (saladMakerNumber == 1)
        {
            double selfPickedGreenPepperWeight = randDouble(0.8 * greenPepperWeight, 1.2 * greenPepperWeight);
            chefBook->totalPickedOnionWeightForSaladMaker1 += chefBook->currentPickedOnionWeight;
            chefBook->totalSelfPickedGreenPepperWeightForSaladMaker1 += selfPickedGreenPepperWeight;
            chefBook->totalPickedTomatoWeightForSaladMaker1 += chefBook->currentPickedTomatoWeight;
        }
        else if (saladMakerNumber == 2)
        {
            double selfPickedOnionWeight = randDouble(0.8 * onionsWeight, 1.2 * onionsWeight);
            chefBook->totalSelfPickedOnionWeightForSaladMaker2 += selfPickedOnionWeight;
            chefBook->totalPickedGreenPepperWeightForSaladMaker2 += chefBook->currentPickedGreenPepperWeight;
            chefBook->totalPickedTomatoWeightForSaladMaker2 += chefBook->currentPickedTomatoWeight;
        }

        //printf("SALAD MAKER %d MAKING SALAD!!\n", saladMakerNumber);
        logString("Salad Maker " + to_string(saladMakerNumber) + " is making salad.");
        double saladMakingTime = randDouble(salmkrtime * 0.8, salmkrtime);
        sleep(saladMakingTime);
        chefBook->SaladMakerTotalTimeWorking[saladMakerNumber] += saladMakingTime;

        chefBook->NumberOfTotalSaladsMadeBySaladMaker[saladMakerNumber] += 1;

        chefBook->isSaladMakerDoingWork[saladMakerNumber] = false;

        //printf("SALAD MAKER %d DONE MAKING SALAD!!\n", saladMakerNumber);
        logString("Salad Maker " + to_string(saladMakerNumber) + " finished making salad.");

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