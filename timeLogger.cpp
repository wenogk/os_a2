
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

const string currentDateTime()
{
    //This function was taken from https://stackoverflow.com/a/10467633
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

bool twoOrMoreSaladMakersDoingWork(ChefBook *chefBook)
{
    return ((chefBook->isSaladMakerDoingWork[0] && chefBook->isSaladMakerDoingWork[1]) || (chefBook->isSaladMakerDoingWork[0] && chefBook->isSaladMakerDoingWork[2]) || (chefBook->isSaladMakerDoingWork[1] && chefBook->isSaladMakerDoingWork[2]));
}

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

    FILE *log = fopen("timeLogger.txt", "at");
    if (!log)
        log = fopen("timeLogger.txt", "wt");
    if (!log)
    {
        perror("can not open logfile.txt for writing.\n");
        return 1; // bail out if we can't log
    }

    if (chefBook == (void *)-1)
    {
        perror("Shared memory attach");
        return 1;
    }
    else
    {
        printf("timeLogger %d shmid\n", shmid);
    }

    while (1)
    {
        if (twoOrMoreSaladMakersDoingWork(chefBook))
        {

            string startTime = currentDateTime(); //start time
            while (twoOrMoreSaladMakersDoingWork(chefBook))
                ;

            string endTime = currentDateTime(); //end timer

            string stringToAppend = startTime + " to " + endTime + "\n";
            fprintf(log, "%s", stringToAppend.c_str()); //append start and end time to file
        }
    }

    int detatch = shmdt(chefBook); /* Detach segment */

    if (detatch < 0)
    {
        perror(" Detachment .\n");
        return 1;
    }

    fclose(log);

    return 0;
}