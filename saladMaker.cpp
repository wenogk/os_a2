
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int opt;
    bool flagS = false; //number of salads flag
    bool flagM = false; //chhef time flag

    char *s_opt = 0;
    char *m_opt = 0;
    while ((opt = getopt(argc, argv, "s:m:")) != -1)
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
        case '?': // unknown option...
            printf("Unknown Option %s\n", opt);
            break;
        }
    }

    if (!flagS || !flagM)
    {
        printf("Argument error. SaladMaker should be invoked as follows: ./saladmaker -m salmkrtime -s shmid \n");
        return 1;
    }
    //./saladmaker -m salmkrtime -s shmid
    printf("I am a saladmaker\n");

    return 0;
}