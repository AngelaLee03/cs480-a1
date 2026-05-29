#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fstream>
#include "bots.h"

using namespace std;

sem_t FLAG;

void* botFunction(void* arg)
{
    int id = *((int*)arg);

    for(int i = 0; i < 8; i++)
    {
        if(id % 2 == 0)
            sleep(2);
        else
            sleep(3);

        sem_wait(&FLAG);

        ofstream file("QUOTE.txt", ios::app);

        if(id % 2 == 0)
        {
            file << "Thread ID " << id
                 << ": \"Controlling complexity is the essence of computer programming.\" "
                 << "--Brian Kernighan\r\n";
        }
        else
        {
            file << "Thread ID " << id
                 << ": \"Computer science is no more about computers than astronomy is about telescopes.\" "
                 << "--Edsger Dijkstra\r\n";
        }

        cout << "Thread " << id << " is running" << endl;

        file.close();

        sem_post(&FLAG);
    }

    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[7];
    int ids[7];

    ofstream file("QUOTE.txt");
    file << "Process ID: " << getpid() << "\r\n";
    file.close();

    sem_init(&FLAG, 0, 1);

    for(int i = 0; i < 7; i++)
    {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, botFunction, &ids[i]);
    }

    for(int i = 0; i < 7; i++)
    {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&FLAG);

    cout << "All threads finished successfully." << endl;

    return 0;
}
