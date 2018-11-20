#ifndef MONITOR_H
#define MONITOR_H
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#include <pthread.h>
#include <string>
using namespace std;

class Monitor
{
    public:
        Monitor();
        void arrive(void *args);
        void cross(void *args);
        void leave(void *args);
        void check();
        void destruct();

    private:
        void initialize();
        int direction[4];
        int waiting[4];
        int crossVar;
        struct bat {
            string dir;
            int batID;
            int right;
            int left;
            int opposite;
            int dirVar;
        };
        pthread_mutex_t monitorLock;

        pthread_mutex_t crossLcok;
        pthread_cond_t first[4];
        pthread_cond_t sameDirection[4];

};

#endif // MONITOR_H
