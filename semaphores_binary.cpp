#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;
#include <queue>
class BinarySemaphore
{
private:
    int value; // 0 or 1
    mutex mtx;
    condition_variable cv;

public:
    BinarySemaphore(int init = 1)
    { // Binary semaphore → initialize with 1
        value = init;
    }

    void wait()
    {
        unique_lock<mutex> lock(mtx);
        while (value == 0)
        { // Block if semaphore is 0
            cv.wait(lock);
        }
        value = 0; // Acquire lock
    }

    void signal()
    {
        unique_lock<mutex> lock(mtx);
        value = 1;       // Release lock
        cv.notify_one(); // Wake one waiting thread
    }
};

BinarySemaphore sem(1); // Global semaphore

void printTask(const string &task)
{
    sem.wait(); // Enter critical section
    cout << "Task started: " << task << endl;

    // Simulate work
    this_thread::sleep_for(chrono::seconds(1));

    cout << "Task finished: " << task << endl;
    sem.signal(); // Exit critical section
}

int main()
{
    thread t1(printTask, "Thread 1");
    thread t2(printTask, "Thread 2");
    thread t3(printTask, "Thread 3");

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
