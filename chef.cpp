
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

int tomatoWeight = 80;
int greenPepperWeight = 50;
int onionsWeight = 30;

struct Kitchen
{
    int cnt;
    int complete;
    char buf[20];
};

int main()
{
    int shmid, numtimes;
    struct Kitchen *kitchenData;
    char *bufptr;
    int spaceavailable;
    shmid = shmget(IPC_PRIVATE, sizeof(struct Kitchen), 0644 | IPC_CREAT);
    if (shmid == -1)
    {
        perror("Shared memory");
        return 1;
    }

    // Attach to the segment to get a pointer to it.
    kitchenData = (Kitchen *)shmat(shmid, NULL, 0);

    if (kitchenData == (void *)-1)
    {
        perror("Shared memory attach");
        return 1;
    }
    else
    {
        printf("shmid %d\n", shmid);
    }
    kitchenData->cnt = 2058;
    // chef
    printf("I am a chef\n");
    printf("\t tomatoWeight: %d", tomatoWeight);

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