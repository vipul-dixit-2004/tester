#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>

using namespace std;

/*
 * Counting semaphore implemented using mutex + condition_variable.
 * - count: available permits
 * - wait(): block until count > 0, then decrement
 * - signal(): increment count and notify one waiting thread
 */
class Semaphore {
private:
    mutex mtx;
    condition_variable cv;
    int count; // number of permits

public:
    // Initialize with initial permits (>= 0)
    Semaphore(int initial = 0) : count(initial) {}

    // P operation (wait)
    void wait() {
        unique_lock<mutex> lock(mtx);
        // use while to protect against spurious wakeups
        while (count == 0) {
            cv.wait(lock);
        }
        // consume one permit
        --count;
    }

    // V operation (signal)
    void signal() {
        {
            lock_guard<mutex> lock(mtx);
            ++count;
        }
        // notify one waiter
        cv.notify_one();
    }

    // Non-blocking attempt to acquire a permit. Returns true if acquired.
    bool try_wait() {
        lock_guard<mutex> lock(mtx);
        if (count > 0) {
            --count;
            return true;
        }
        return false;
    }
};

/*
 * Binary semaphore (0 or 1). It's effectively a Semaphore with max 1.
 * If you need strict "mutex-like" ownership semantics, prefer std::mutex.
 */
class BinarySemaphore {
private:
    Semaphore sem;

public:
    BinarySemaphore(bool initially_locked = false)
        : sem(initially_locked ? 0 : 1) {}

    void wait() { sem.wait(); }
    void signal() { sem.signal(); }
    bool try_wait() { return sem.try_wait(); }
};

/* --------------------
   Example 1: Producer-Consumer using a counting semaphore
   - emptySlots: counting semaphore tracking free slots (initially buffer_size)
   - filledSlots: counting semaphore tracking items available (initially 0)
   - mutex protects the buffer vector
   -------------------- */
void producer_consumer_demo() {
    const int buffer_size = 5;
    vector<int> buffer;
    mutex buf_mtx;

    Semaphore emptySlots(buffer_size);
    Semaphore filledSlots(0);

    // producer
    auto producer = [&](int id, int produce_count){
        for (int i = 0; i < produce_count; ++i) {
            // wait for an empty slot
            emptySlots.wait();

            // produce (critical section)
            {
                lock_guard<mutex> lock(buf_mtx);
                int item = id * 100 + i;
                buffer.push_back(item);
                cout << "[Producer " << id << "] produced " << item
                     << " (buffer size = " << buffer.size() << ")\n";
            }

            // signal that there's a filled slot
            filledSlots.signal();

            // simulate work
            this_thread::sleep_for(chrono::milliseconds(50));
        }
    };

    // consumer
    auto consumer = [&](int id, int consume_count){
        for (int i = 0; i < consume_count; ++i) {
            // wait for an available item
            filledSlots.wait();

            int item;
            {
                lock_guard<mutex> lock(buf_mtx);
                item = buffer.back();
                buffer.pop_back();
                cout << "    [Consumer " << id << "] consumed " << item
                     << " (buffer size = " << buffer.size() << ")\n";
            }

            // signal that there is an empty slot
            emptySlots.signal();

            // simulate processing time
            this_thread::sleep_for(chrono::milliseconds(80));
        }
    };

    thread p1(producer, 1, 8);
    thread p2(producer, 2, 8);
    thread c1(consumer, 1, 8);
    thread c2(consumer, 2, 8);

    p1.join(); p2.join(); c1.join(); c2.join();
}

/* --------------------
   Example 2: Binary semaphore used as a simple lock
   (demonstration only — prefer std::mutex for mutual exclusion)
   -------------------- */
void binary_semaphore_demo() {
    BinarySemaphore binSem(false); // initially unlocked
    int shared_counter = 0;

    auto worker = [&](int id) {
        for (int i = 0; i < 5; ++i) {
            binSem.wait(); // enter critical section
            int local = shared_counter;
            ++local;
            this_thread::sleep_for(chrono::milliseconds(10)); // simulate work
            shared_counter = local;
            cout << "[Worker " << id << "] incremented to " << shared_counter << "\n";
            binSem.signal(); // leave critical section

            this_thread::sleep_for(chrono::milliseconds(30));
        }
    };

    thread t1(worker, 1);
    thread t2(worker, 2);
    t1.join(); t2.join();
    cout << "Final shared_counter = " << shared_counter << "\n";
}

int main() {
    cout << "=== Producer-Consumer demo (counting semaphore) ===\n";
    producer_consumer_demo();

    cout << "\n=== Binary semaphore demo ===\n";
    binary_semaphore_demo();

    return 0;
}
