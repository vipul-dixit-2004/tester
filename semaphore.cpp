#include <iostream>
#include <queue>
#include <unistd.h>
using namespace std;


class Process {
public:
    int id;  // Unique identifier for the process
    
    // Constructor with id
    Process(int id) : id(id) {}
    
    // Default constructor with a special id (-1) for empty Process objects
    Process() : id(-1) {}
};

template <typename T>
class Queue {
private:
    std::queue<T> q;

public:
    void push(T item) {
        q.push(item);
    }

    T pop() {
        if (!q.empty()) {
            T item = q.front();
            q.pop();
            return item;
        }
        return T(); 
    }

    bool isEmpty() {
        return q.empty();
    }
};

struct Semaphore {
    int value;  
    Queue<Process> q;  

    Semaphore(int initValue) : value(initValue) {}
};


void P(Semaphore &s, Process &p) {
    cout << "Process " << p.id << " tries to acquire resource...\n";
    s.value = s.value - 1;
    if (s.value < 0) {
        cout << "Process " << p.id << " is blocked (no resources available).\n";
        s.q.push(p);  // Block the process and add to the queue
    } else {
        cout << "Process " << p.id << " can proceed (resource acquired).\n";
    }
}

void V(Semaphore &s) {
    s.value = s.value + 1;
    if (s.value <= 0) {
        Process p = s.q.pop();
        if (p.id != -1) {
            cout << "Process " << p.id << " is unblocked (resource released).\n";
        }
    } else {
        cout << "Semaphore value increased (resource released).\n";
    }
}

// Function to simulate processes trying to acquire and release the semaphore
void simulateProcess(Semaphore &sem, Process &p) {
    // Simulate process trying to acquire the semaphore (wait)
    P(sem, p);
    
    // Simulate process doing some work (i.e., performing a task for a while)
    cout << "Process " << p.id << " is performing some work...\n";

    // Once done, simulate process releasing the semaphore (signal)
    V(sem);
}

int main() {
    Semaphore sem(3);

    // Create processes
    const int numProcesses = 5;
    Process processes[numProcesses];
    for (int i = 0; i < numProcesses; ++i) {
        processes[i] = Process(i + 1);
    }

    // Simulate each process trying to acquire and release the semaphore sequentially
    for (int i = 0; i < numProcesses; ++i) {
        simulateProcess(sem, processes[i]);
    }

    return 0;
}
