#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cthread.h"
#include "config_file.h"


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
    CConfigFile cf;
    int32_t    int1, int2;
    double double1;
    std::string string1;
    bool   bool1;
    
    cf.read("test_spec.txt");
    cf.dump_specs();

    try
    {
        cf.getv("multi", "ifsb", &int1, &double1, &string1, &bool1);
        printf("%i : %1.5lf : '%s' : %i\n", int1, double1, string1.c_str(), bool1);

        cf.get("two_ints", &int1, &int2);
        printf("%i %i\n", int1, int2);
        
        std::vector<double> dv;
        cf.get("float_list", &dv);
        printf("float_list has %i elements\n", (int)dv.size());
        for (auto x : dv) printf("%1.3lf ", x);
        printf("\n");

    }

    catch(std::runtime_error& e)
    {
        printf("Caught runtime_error : %s\n", e.what());
    }
    
    printf("Done");

/*


    srand(time(NULL));

    thread1.spawn();
    thread2.spawn();

    thread1.join();
    thread2.join();
*/

    return 0;
}

