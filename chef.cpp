
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>

int main()
{

    // chef
    printf("I am a chef\n");

    // 3 SaladMakers, therefore create 3 salad maker processes
    pid_t pids[3];

    for (int i = 0; i < 3; i++)
    {
        pids[i] = fork();
        if (pids[i] == 0)
        {
            char *sorterData[3] = {"./saladMaker", "saladMaker", (char *)NULL};
            if (execv(sorterData[0], sorterData) == -1)
            {
                perror("Error creating salad maker process");
            }
        }
    }

    for (int i = 0; i < 3; i++)
    {
        wait(NULL); // wait for each salad maker
    }

    return 0;
}