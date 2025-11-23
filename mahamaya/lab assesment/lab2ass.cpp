#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <unistd.h>     // for fork()
#include <sys/wait.h>   // for waitpid()

using namespace std;

mutex cout_mutex;
mutex sem;  // binary semaphore (like mutex)

// Process simulation function
void runProcess(int pid, bool isSystemCall) {
    sem.lock(); // enter critical section

    {
        lock_guard<mutex> lock(cout_mutex);
        if (isSystemCall)
            cout << "Process " << pid << " is a SYSTEM CALL !!!!! Preempting current batch!\n";
        else
            cout << "Process " << pid << " is running...\n";
    }

    if (isSystemCall) {
        pid_t child = fork();

        if (child < 0) {
            lock_guard<mutex> lock(cout_mutex);
            cerr << "Fork failed for system call process " << pid << "!\n";
        } 
        else if (child == 0) { 
            // Child process
            lock_guard<mutex> lock(cout_mutex);
            cout << "  [Child of system call " << pid << "] running with PID: " << getpid() << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
            cout << "  [Child of system call " << pid << "] finished.\n";
            _exit(0);
        } 
        else { 
            // Parent process waits for child
            cout<<" parent process  "<<pid<<"]running .\n";
            waitpid(child, NULL, 0);
        }
    } 
    else {
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    {
        lock_guard<mutex> lock(cout_mutex);
        if (isSystemCall)
            cout << "System call process " << pid << " completed.\n";
        else
            cout << "Process " << pid << " completed.\n";
    }

    sem.unlock(); // exit critical section
}

int main() {
    int n, sysN, batchSize;
    cout << "Enter total number of processes: ";
    cin >> n;

    cout << "Enter number of system calls: ";
    cin >> sysN;

    vector<int> systemCall(sysN);
    cout << "Enter the process numbers which are system calls (1-based indexing):\n";
    for (int i = 0; i < sysN; i++)
        cin >> systemCall[i];

    cout << "Enter batch size: ";
    cin >> batchSize;

    vector<bool> isSys(n + 1, false);
    for (int id : systemCall)
        if (id >= 1 && id <= n)
            isSys[id] = true;

    cout << "\nStarting---------------------------------------------------\n\n";

    for (int i = 1; i <= n; i += batchSize) {
        int end = min(i + batchSize - 1, n);

        bool hasSysCall = false;
        for (int j = i; j <= end; j++)
            if (isSys[j]) hasSysCall = true;

        if (hasSysCall) {
            // First run all system calls in this batch
            for (int j = i; j <= end; j++)
                if (isSys[j]) thread(runProcess, j, true).join();

            // Then run normal processes
            for (int j = i; j <= end; j++)
                if (!isSys[j]) thread(runProcess, j, false).join();
        } else {
            // Normal batch without system call
            for (int j = i; j <= end; j++)
                thread(runProcess, j, false).join();
        }
    }

    cout << "\nEnding-----------------------------------------------------\n";
    return 0;
}
