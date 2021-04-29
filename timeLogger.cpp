
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include "kitchen.h"
#include <fcntl.h>    /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <semaphore.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <cstring>
#include <fstream>
#include <iostream>

bool twoOrMoreSaladMakersDoingWork(ChefBook *chefBook)
{
    return ((chefBook->isSaladMakerDoingWork[0] && chefBook->isSaladMakerDoingWork[1]) || (chefBook->isSaladMakerDoingWork[0] && chefBook->isSaladMakerDoingWork[2]) || (chefBook->isSaladMakerDoingWork[1] && chefBook->isSaladMakerDoingWork[2]));
}

int main(int argc, char *argv[])
{
    ofstream starter("intersectingTimeLogger.txt", ios::out);
    if (!starter)
    {
        cout << "Error Opening File" << endl;
        return -1;
    }
    starter << "TIME RANGES THAT TWO OR MORE SALAD MAKERS WERE WORKING AT THE SAME TIME\n"
            << endl;
    starter.close();

    ofstream writer("intersectingTimeLogger.txt", ios::app);
    //ios::out
    if (!writer)
    {
        cout << "Error Opening File" << endl;
        return -1;
    }
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
        //printf("timeLogger joined %d shmid\n", shmid);
    }

    while (1)
    {
        if (twoOrMoreSaladMakersDoingWork(chefBook))
        {

            time_t startTime = time(NULL);
            // ctime() used to give the present time
            while (twoOrMoreSaladMakersDoingWork(chefBook))
                ;
            time_t endTime = time(NULL);
            char *startTime_no_newline;
            char *endTime_no_newline;

            startTime_no_newline = strdup(ctime(&startTime)); //myctime[ strlen(myctime) - 1 ] = '\0';
            startTime_no_newline[strlen(startTime_no_newline) - 1] = '\0';

            endTime_no_newline = strdup(ctime(&endTime)); //myctime[ strlen(myctime) - 1 ] = '\0';
            endTime_no_newline[strlen(endTime_no_newline) - 1] = '\0';

            std::string stringToAppend;             // an empty string
            stringToAppend += startTime_no_newline; // append the first character
            stringToAppend += " to ";
            stringToAppend += endTime_no_newline; // append the second character
            // ctime() used to give the present time

            writer << "\t" << stringToAppend << endl;
        }
    }
    writer.close();
    int detatch = shmdt(chefBook); /* Detach segment */

    if (detatch < 0)
    {
        perror(" Detachment .\n");
        return 1;
    }

    return 0;
}