#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fstream>
#include "bots.h"

using namespace std;

// Global semaphore used to control access to QUOTE.txt
sem_t FLAG;

/*
 * Thread function executed by each bot thread
 * Each thread writes its assigned quote to QUOTE.txt
 * while using a semaphore to prevent file corruption
 */
void* botFunction(void* arg)
{
    // Get thread ID passed from main()
    int id = *((int*)arg);

    // Each thread runs a total of 8 times
    for(int i = 0; i < 8; i++)
    {
        // Even numbered threads once every 2 seconds
        if(id % 2 == 0)
            sleep(2);

        // Odd numbered threads once every 3 seconds
        else
            sleep(3);

        // Wait for access to shared file
        sem_wait(&FLAG);

        // Open QUOTE.txt in append mode
        ofstream file("QUOTE.txt", ios::app);

        // Even threads write the Brian Kernighan quote
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

        // Print the thread activity to console
        cout << "Thread " << id << " is running" << endl;

        // Close the file after writing
        file.close();

        // Release semaphore so another thread can access the file
        sem_post(&FLAG);
    }

    // Exit the thread cleanly
    pthread_exit(NULL);
}

/*
 * Main function:
 * Creates shared file, initializes semaphore,
 * creates threads, waits for completion,
 * destroys semaphore, and exits.
 */
int main()
{
    // Array of 7 thread objects
    pthread_t threads[7];

    // Array holding thread IDs
    int ids[7];

    // Create QUOTE.txt and write process ID
    ofstream file("QUOTE.txt");
    file << "Process ID: " << getpid() << "\r\n";
    file.close();

    // Initialize semaphore with value 1
    sem_init(&FLAG, 0, 1);

    // Create 7 threads
    for(int i = 0; i < 7; i++)
    {
        // Thread IDs will be numbered 1 through 7
        ids[i] = i + 1;

        // Create thread
        pthread_create(&threads[i], NULL, botFunction, &ids[i]);
    }

    // Wait for all of the threads to complete
    for(int i = 0; i < 7; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Destroy semaphore after all threads finish
    sem_destroy(&FLAG);

    // Friendly exit message
    cout << "All threads finished successfully." << endl;

    return 0;
}
