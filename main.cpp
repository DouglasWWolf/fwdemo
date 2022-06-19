#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cthread.h>
#include <time.h>


// In order to create a thread object, we derive from CThread
class CWorker : public CThread
{
    virtual void main(void* p1=0, void* p2=0, void* p3=0);
};

void CWorker::main(void* p1, void* p2, void* p3)
{
    for (int i=0; i<10; ++i)
    {
        printf("Thread #%i says hello %i\n", m_id, i);
        usleep(rand() % 1000000);
    }    
}

CWorker thread1, thread2;

int main()
{
    srand(time(NULL));

    thread1.spawn();
    thread2.spawn();

    thread1.join();
    thread2.join();


    return 0;
}

