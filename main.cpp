#include <iostream>
#include "Monitor.h"
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <ctime>

using namespace std;

Monitor m = Monitor();
struct bat {
    string dir;
    int batID;
    int right;
    int left;
    int opposite;
    int dirVar;
};
void* batTask (void* args) {
    bat *b = (bat*)args;
    m.arrive(args);
    m.cross(args);
    m.leave(args);

}

void *batManTask (void* id) {
    while (true) {
        m.check();
        sleep(2);
    }
}


void *errorExec (void* id) {
}


int main()
{
    string input;
    cout << "Enter Bats : ";
    cin >> input;
    srand(time(NULL));
    pthread_t batManThread;
    pthread_t batThreads[input.length()];
    bat batArgs[input.length()];

    int status = pthread_create(&batManThread, NULL, batManTask, NULL);
    if (status > 0) {
        cout << "Failed to create thread" << endl;
    }


    for (int i = 0; i <input.length(); i++) {

        switch(input.at(i)) {
            case 'n':
                batArgs[i].batID = i+1;
                batArgs[i].dir = "North";
                batArgs[i].dirVar = NORTH;
                batArgs[i].left = EAST;
                batArgs[i].right = WEST;
                batArgs[i].opposite = SOUTH;
                break;
            case 's':
                batArgs[i].batID = i+1;
                batArgs[i].dir = "South";
                batArgs[i].dirVar = SOUTH;
                batArgs[i].left = WEST;
                batArgs[i].right = EAST;
                batArgs[i].opposite = NORTH;
                break;
            case 'e':
                batArgs[i].batID = i+1;
                batArgs[i].dir = "East";
                batArgs[i].dirVar = EAST;
                batArgs[i].left = SOUTH;
                batArgs[i].right = NORTH;
                batArgs[i].opposite = WEST;
                break;
            case 'w':
                batArgs[i].batID = i+1;
                batArgs[i].dir = "West";
                batArgs[i].dirVar = WEST;
                batArgs[i].left = NORTH;
                batArgs[i].right = SOUTH;
                batArgs[i].opposite = EAST;
                break;
            default:
                pthread_create(&batThreads[i], NULL, errorExec, (void*)&batArgs[i]);
                continue;


        }
        status = pthread_create(&batThreads[i], NULL, batTask, (void*)&batArgs[i]);
        if (status > 0) {
            cout << "Failed to create thread" << endl;
        }
    }

    for (int i = 0; i < input.length(); i++) {
        pthread_join(batThreads[i], NULL);
    }

    m.destruct();
    return 0;
}
