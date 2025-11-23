#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

class CountingSemaphore {
private:
    int count;
    mutex mtx;
    condition_variable cv;

public:
    CountingSemaphore(int initialCount) : count(initialCount) {}

    void wait() {
        unique_lock<mutex> lock(mtx);

        cout<<"count is : "<<count<<" \n";

        cv.wait(lock, [&]() { return count > 0; });
        --count;

    }

    void signal() {
         cout<<"count is : "<<count<<" \n";
        unique_lock<mutex> lock(mtx);
        ++count;
        cv.notify_one();
    }
};

void worker(int id, CountingSemaphore& sem) {
    cout << "Worker " << id << " is waiting to enter the critical section...\n";
    sem.wait();
    cout << "Worker " << id << " has entered the critical section.\n";


    this_thread::sleep_for(chrono::seconds(2));

    cout << "Worker " << id << " is leaving the critical section.\n";
    sem.signal();
}

int main() {
     int maxConcurrent = 3;
    CountingSemaphore sem(maxConcurrent);

    thread t1(worker, 1, ref(sem));
    thread t2(worker, 2, ref(sem));
    thread t3(worker, 3, ref(sem));
    thread t4(worker, 4, ref(sem));

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
