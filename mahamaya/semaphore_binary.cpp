#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;

class BinarySemaphore {
private:
    mutex mtx;          // matlab ek time pr available ki value ek process hi check kr ske ....dono process ek sath check nahi kr skti
    bool available;     // true if semaphore is free, false agr taken 

public:
    BinarySemaphore(bool initial = true) : available(initial) {}

    void wait() {
        while (true) { // infinite loop ke andr fasi hai process
            mtx.lock();
            if (available) {
                available = false;  // take the semaphore
                mtx.unlock();
                break;             // exit loop and enter critical section
            }
            mtx.unlock();
            this_thread::sleep_for(chrono::milliseconds(10)); // wait a bit and try again
        }
    }

    void signal() {
        mtx.lock();
        available = true;   // release semaphore
        mtx.unlock();
    }
};

// Simulate a process trying to enter critical section
void process(BinarySemaphore& sem, int id) {
    cout << "Process " << id << " is trying to enter critical section...\n";
    sem.wait();  // wait to enter critical section

    // Critical section start
    cout << "Process " << id << " ENTERED critical section.\n";
    this_thread::sleep_for(chrono::seconds(1));  // just 1 second maan liya critical section chl rha ...code nahi likha 
    cout << "Process " << id << " LEAVING critical section.\n";
    // Critical section end ho gya ab release kr do semaphore ko 
   sem.signal(); // release semaphore
}

int main() {
    BinarySemaphore sem(true);  // semaphore initially available

    // Start two threads simulating two processes trying to enter critical section
    thread t1(process, ref(sem), 1);
    thread t2(process, ref(sem), 2);
    thread t3(process,ref(sem),3);
    thread t4(process,ref(sem),4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    cout << "All processes completed.\n";
    return 0;
}
