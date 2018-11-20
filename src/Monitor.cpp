#include "Monitor.h"
#include <pthread.h>
#include <string>
#include <iostream>
#include <unistd.h>

using namespace std;

Monitor::Monitor() {
    initialize();
}

void Monitor::initialize() {
    crossVar = 0;
    direction[NORTH] = direction[EAST] = 0;
    direction[SOUTH] = direction[WEST] = 0;

    waiting[NORTH] = waiting[EAST] = 0;
    waiting[SOUTH] = waiting[WEST] = 0;


    pthread_mutex_init(&monitorLock, NULL);

    pthread_mutex_init(&crossLcok, NULL);

    pthread_cond_init(&first[NORTH], NULL);
    pthread_cond_init(&first[EAST], NULL);
    pthread_cond_init(&first[SOUTH], NULL);
    pthread_cond_init(&first[WEST], NULL);

    pthread_cond_init(&sameDirection[NORTH], NULL);
    pthread_cond_init(&sameDirection[EAST], NULL);
    pthread_cond_init(&sameDirection[SOUTH], NULL);
    pthread_cond_init(&sameDirection[WEST], NULL);

}
void Monitor::arrive(void *args) {
    pthread_mutex_lock(&monitorLock);
    bat *b = (bat*)args;
    direction[b->dirVar]++;
    if (direction[b->dirVar] > 1) {
        pthread_cond_wait(&sameDirection[b->dirVar], &monitorLock);
    }
    cout << "Bat " << b->batID << " from " << b->dir << " arrives at crossing" <<endl;
    pthread_mutex_unlock(&monitorLock);
}

void Monitor::cross(void *args) {
    pthread_mutex_lock(&monitorLock);
    bat *b = (bat*)args;

    if (direction[b->right] > 0) {
        waiting[b->dirVar]++;
        cout << "Bat " << b->batID << " from " << b->dir << " waiting on Right" <<endl;
        pthread_cond_wait(&first[b->right], &monitorLock);
        waiting[b->dirVar]--;
    }

    pthread_mutex_lock(&crossLcok);
    crossVar++;
    cout << "Bat " << b->batID << " from " << b->dir << " crossing" <<endl;
    pthread_mutex_unlock(&monitorLock);

    sleep(1);
    crossVar--;
    pthread_mutex_unlock(&crossLcok);

}

void Monitor::leave(void *args) {
    pthread_mutex_lock(&monitorLock);
    bat *b = (bat*)args;
    cout << "Bat " << b->batID << " from " << b->dir << " leaving crossing" <<endl;
    direction[b->dirVar]--;

    if (waiting[b->left] > 0) {
        pthread_cond_signal(&first[b->dirVar]);
    }
    pthread_cond_signal(&sameDirection[b->dirVar]);

    pthread_mutex_unlock(&monitorLock);
}

void Monitor::check() {
    pthread_mutex_lock(&monitorLock);
    if (crossVar == 0 && waiting[NORTH] > 0 && waiting[SOUTH] > 0
        && waiting[EAST] > 0 && waiting[WEST] > 0) {
        cout << "DEADLOCK: BAT jam detected, signalling Random Direction to go" << endl;
        int randomDir = rand() % 4;
        pthread_cond_signal(&first[randomDir]);
    }

    pthread_mutex_unlock(&monitorLock);
}

void Monitor::destruct () {
    pthread_mutex_destroy(&monitorLock);

    pthread_mutex_destroy(&crossLcok);

    pthread_cond_destroy(&first[NORTH]);
    pthread_cond_destroy(&first[SOUTH]);
    pthread_cond_destroy(&first[EAST]);
    pthread_cond_destroy(&first[WEST]);

    pthread_cond_destroy(&sameDirection[NORTH]);
    pthread_cond_destroy(&sameDirection[SOUTH]);
    pthread_cond_destroy(&sameDirection[EAST]);
    pthread_cond_destroy(&sameDirection[WEST]);

}
