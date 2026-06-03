#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fstream>
#include <cstdlib>
#include "bots.h"

using namespace std;

#define NUM_THREADS 7

// Global semaphore used to control access to QUOTE.txt
sem_t FLAG;

/*
 * Thread function executed by each bot thread.
 * Each thread writes its assigned quote to QUOTE.txt
 * while using a semaphore to prevent file corruption.
 */
void* botFunction(void* arg)
{
    // Get thread ID passed from main()
    int id = *((int*)arg);

    // Each thread runs a total of 8 times
    for(int i = 0; i < 8; i++)
    {
        // Even numbered threads run every 2 seconds
        if(id % 2 == 0)
            sleep(2);
        else
            sleep(3);

        // Wait for access to the shared file
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

        // Odd threads write the Edsger Dijkstra quote
        else
        {
            file << "Thread ID " << id
                 << ": \"Computer science is no more about computers than astronomy is about telescopes.\" "
                 << "--Edsger Dijkstra\r\n";
        }

        // Print thread activity to console
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
    // Array of thread objects
    pthread_t threads[NUM_THREADS];

    // Array holding thread IDs
    int ids[NUM_THREADS];

    // Variable used for error checking
    int rc;

    // Create QUOTE.txt and write process ID
    ofstream file("QUOTE.txt");
    file << "Process ID: " << getpid() << "\r\n";
    file.close();

    // Initialize semaphore with value 1
    rc = sem_init(&FLAG, 0, 1);

    if(rc)
    {
        cout << "ERROR; semaphore initialization failed." << endl;
        exit(-1);
    }

    // Create threads
    for(int i = 0; i < NUM_THREADS; i++)
    {
        // Thread IDs will be numbered 1 through 7
        ids[i] = i + 1;
        // Informs threads are being created
        cout << "Creating thread, in main(): " 
            << ids[i] << endl;

        // Create thread
        rc = pthread_create(&threads[i], NULL, botFunction, &ids[i]);

        // Check for thread creation errors
        if(rc)
        {
            cout << "ERROR; return code from pthread_create() is "
                 << rc << endl;
            exit(-1);
        }
    }

    // Wait for all threads to complete
    for(int i = 0; i < NUM_THREADS; i++)
    {
        rc = pthread_join(threads[i], NULL);

        // Check for thread join errors
        if(rc)
        {
            cout << "ERROR; return code from pthread_join() is "
                 << rc << endl;
            exit(-1);
        }
    }

    // Destroy semaphore after all threads finish
    rc = sem_destroy(&FLAG);

    if(rc)
    {
        cout << "ERROR; semaphore destruction failed." << endl;
        exit(-1);
    }

    // Friendly exit message
    cout << "All threads finished successfully." << endl;

    return 0;
}
