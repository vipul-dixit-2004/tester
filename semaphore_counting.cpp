#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

class CountingSemaphore
{
private:
    int value; // current permits
    mutex mtx;
    condition_variable cv;

public:
    CountingSemaphore(int count)
    { // initialize with available resources
        value = count;
    }

    void wait()
    {
        unique_lock<mutex> lock(mtx);
        while (value == 0)
        { // if no permits left, wait
            cv.wait(lock);
        }
        value--; // use one permit
    }

    void signal()
    {
        unique_lock<mutex> lock(mtx);
        value++;         // release permit
        cv.notify_one(); // wake up one waiting thread
    }
};

CountingSemaphore sem(2); // allow 2 threads to run at same time

void worker(int id)
{
    sem.wait(); // acquire permit

    cout << "Thread " << id << " entered critical section.\n";
    this_thread::sleep_for(chrono::seconds(1)); // simulate work
    cout << "Thread " << id << " exiting critical section.\n";

    sem.signal(); // release permit
}

int main()
{
    thread t1(worker, 1);
    thread t2(worker, 2);
    thread t3(worker, 3);
    thread t4(worker, 4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
