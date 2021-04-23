
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
#include <iostream>
#include <fstream>

//default weights of the veggies
double tomatoWeight = 80.0;
double greenPepperWeight = 50.0;
double onionsWeight = 30.0;

void logString(string logString)
{ //function that logs string into the log file with the current date and time.
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

//random numebr functions taken from https://stackoverflow.com/questions/10776073/random-double-between-min-and-max

int main(int argc, char *argv[])
{
    srand(time(0));

    //semaphores for salad maker 2
    sem_t *Tomato_GreenPepper_semaphore_empty;
    sem_t *Tomato_GreenPepper_semaphore_full;
    sem_t *Tomato_GreenPepper_semaphore_mutex;
    sem_t *Tomato_GreenPepper_semaphore_done;

    //semaphores for salad maker 1
    sem_t *Tomato_Onions_semaphore_empty;
    sem_t *Tomato_Onions_semaphore_full;
    sem_t *Tomato_Onions_semaphore_mutex;
    sem_t *Tomato_Onions_semaphore_done;

    //semaphores for salad maker 0
    sem_t *GreenPepper_Onions_semaphore_empty;
    sem_t *GreenPepper_Onions_semaphore_full;
    sem_t *GreenPepper_Onions_semaphore_mutex;
    sem_t *GreenPepper_Onions_semaphore_done;

    int opt;
    bool flagN = false;          //number of salads flag
    bool flagM = false;          //chhef time flag
    bool flagS = false;          //salad maker time if running through chef fork
    bool flagStandalone = false; //means the chef will not fork the salad makers, you will have to run those programes yourself

    //variables to store the option values
    char *n_opt = 0;
    char *m_opt = 0;
    char *s_opt = 0;

    //opening the log file and logging first line and removing previous content if there was any.
    ofstream starter("logFile.txt", ios::out);
    if (!starter)
    {
        cout << "Error Opening File" << endl;
        return -1;
    }
    starter << "LOGS OF SALAD MAKING OF ROMENO'S KITCHEN" << endl
            << "---------------" << endl
            << endl;
    starter.close();

    //get the arguments for the chef program
    while ((opt = getopt(argc, argv, "n:m:s:standalone:")) != -1)
    { // for each option...
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
        case 's':
            s_opt = optarg;
            flagS = true;
            break;
        case 'standalone':
            flagStandalone = true;
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

    int shmid, numtimes;       // variables for shared memory
    struct ChefBook *chefBook; //data structure we are sharing
    char *bufptr;
    int spaceavailable;
    double cheftime = atof(m_opt);

    shmid = shmget(IPC_PRIVATE, sizeof(struct ChefBook), 0644 | IPC_CREAT); //create shared memory segment
    if (shmid == -1)
    {
        perror("Shared memory");
        return 1;
    }

    chefBook = (ChefBook *)shmat(shmid, NULL, 0); // Attach to the shared memory segment to get a pointer to it.

    if (chefBook == (void *)-1) //if attaching failed, exit and print error
    {
        perror("Shared memory attach");
        return 1;
    }
    else
    {
        printf("shmid %d\n", shmid);
    }

    chefBook->SaladMakerTotalTimeWaiting[0] = 0.0; // Initialize total time waiting for salad maker 0 to 0
    chefBook->SaladMakerTotalTimeWaiting[1] = 0.0; // Initialize total time waiting for salad maker 1 to 0
    chefBook->SaladMakerTotalTimeWaiting[2] = 0.0; // Initialize total time waiting for salad maker 2 to 0

    //Next, we open all the semaphores necessary (4 for each salad maker)

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
    timeLoggerPid = fork(); // We are forking for the time logger process that will log the times that two or more salad makers are working at the same time
    if (timeLoggerPid == 0)
    {
        char shmidChar[15];
        sprintf(shmidChar, "%d", shmid);
        char *sorterData[4] = {"./timeLogger", "-s", shmidChar, (char *)NULL}; // passing the shared memory segment id to the time logger process
        if (execv(sorterData[0], sorterData) == -1)
        {
            perror("Error creating salad maker process");
        }
    }

    if (!flagStandalone) //if the -standalone option is used, we will not fork the salad makers and the user will have to call the ./saladMaker programs themselves
    {
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
                char saladMakerTime[15];
                sprintf(shmidChar, "%d", shmid);
                if (flagS)
                {
                    sprintf(saladMakerTime, "%f", atof(s_opt));
                }
                else
                {
                    sprintf(saladMakerTime, "%f", 2.0);
                }
                char *sorterData[8] = {"./saladMaker", "-m", saladMakerTime, "-s", shmidChar, "-n", saladMakerNumberChar, (char *)NULL};
                if (execv(sorterData[0], sorterData) == -1)
                {
                    perror("Error creating salad maker process");
                }
            }
        }
    }

    int totalSaladsNeeded = atoi(n_opt);
    int totalVeggiePairsGivenToSaladMakers = 0;

    while (totalVeggiePairsGivenToSaladMakers < totalSaladsNeeded)
    {
        int chosenSaladMakerIndex = randNum(0, 2);
        logString("Chef randomly picked up " + vegetablePairEnumToNormalString(saladMakerNumberToVegetablePairNeeded(chosenSaladMakerIndex)));

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

            chefBook->currentPickedGreenPepperWeight = randDouble(0.8 * greenPepperWeight, 1.2 * greenPepperWeight);
            chefBook->currentPickedOnionWeight = randDouble(0.8 * onionsWeight, 1.2 * onionsWeight);

            logString("Chef placed " + vegetablePairEnumToNormalStringWithWeights(saladMakerNumberToVegetablePairNeeded(chosenSaladMakerIndex), chefBook) + " on Salad Maker " + to_string(chosenSaladMakerIndex) + "\'s workspace.");

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

            logString("Chef waiting for Salad maker " + to_string(chosenSaladMakerIndex) + " to pick up " + vegetablePairEnumToNormalString(saladMakerNumberToVegetablePairNeeded(chosenSaladMakerIndex)));

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

            chefBook->currentPickedOnionWeight = randDouble(0.8 * onionsWeight, 1.2 * onionsWeight);
            chefBook->currentPickedTomatoWeight = randDouble(0.8 * tomatoWeight, 1.2 * tomatoWeight);

            logString("Chef placed " + vegetablePairEnumToNormalStringWithWeights(saladMakerNumberToVegetablePairNeeded(chosenSaladMakerIndex), chefBook) + " on Salad Maker " + to_string(chosenSaladMakerIndex) + "\'s workspace.");

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

            logString("Chef waiting for Salad maker " + to_string(chosenSaladMakerIndex) + " to pick up " + vegetablePairEnumToNormalString(saladMakerNumberToVegetablePairNeeded(chosenSaladMakerIndex)));

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

            chefBook->currentPickedGreenPepperWeight = randDouble(0.8 * greenPepperWeight, 1.2 * greenPepperWeight);
            chefBook->currentPickedTomatoWeight = randDouble(0.8 * tomatoWeight, 1.2 * tomatoWeight);

            logString("Chef placed " + vegetablePairEnumToNormalStringWithWeights(saladMakerNumberToVegetablePairNeeded(chosenSaladMakerIndex), chefBook) + " on Salad Maker " + to_string(chosenSaladMakerIndex) + "\'s workspace.");

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

            logString("Chef waiting for Salad maker " + to_string(chosenSaladMakerIndex) + " to pick up " + vegetablePairEnumToNormalString(saladMakerNumberToVegetablePairNeeded(chosenSaladMakerIndex)));

            if (sem_wait(Tomato_GreenPepper_semaphore_done) < 0)
            {
                perror("sem wait");
                return 1;
            }
        }

        double sleepyTime = randDouble(cheftime * 0.5, cheftime);
        sleep(sleepyTime);

        printf("Serving salad.. \n");
        totalVeggiePairsGivenToSaladMakers += 1;
    }

    while (chefBook->isSaladMakerDoingWork[0] || chefBook->isSaladMakerDoingWork[1] || chefBook->isSaladMakerDoingWork[2])
        ;

    if (!flagStandalone)
    {
        for (int i = 0; i < 3; i++)
        {
            kill(pids[i], SIGTERM); // kill each salad maker
        }
    }

    kill(timeLoggerPid, SIGTERM); //kill the timeLogger process

    logChefBook(chefBook); //log all the relevant data to the user

    //next, log the time ranges where two or more salad makers were working at the same time
    printf("----------------\n");
    std::ifstream file("intersectingTimeLogger.txt");
    std::string str;
    while (std::getline(file, str))
    {
        std::cout << str << endl;
    }
    printf("----------------\n");

    //unlink AND close all semaphores

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