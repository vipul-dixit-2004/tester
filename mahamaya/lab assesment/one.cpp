#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;

mutex mtx;
void runProcess(int pid, bool isSystemCall) {
    lock_guard<mutex> lock(mtx); 

    if (isSystemCall) {
        cout << "Process " << pid << " is a SYSTEM CALL → Preempting current batch!\n";
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "System call process " << pid << " completed.\n";
    } else {
        cout << "Process " << pid << " is running...\n";
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "Process " << pid << " completed.\n";
    }
}

int main() {
    int n, sysN, batchSize;
    cout << "Enter total number of processes: ";
    cin >> n;

    cout << "Enter number of system calls: ";
    cin >> sysN;

    vector<int> systemCall(sysN);
    cout << "Enter the process numbers which are system calls (1-based indexing):\n";
    for (int i = 0; i < sysN; i++) {
        cin >> systemCall[i];
    }

    cout << "Enter batch size: ";
    cin >> batchSize;

    vector<bool> isSys(n + 1, false);
    for (int id : systemCall)
        if (id >= 1 && id <= n)
            isSys[id] = true;

    cout << "\n=== PROCESS EXECUTION STARTS ===\n\n";

    for (int i = 1; i <= n; i += batchSize) {
        int end = min(i + batchSize - 1, n);

        bool hasSysCall = false;
        for (int j = i; j <= end; j++)
            if (isSys[j]) hasSysCall = true;

        if (hasSysCall) {
            for (int j = i; j <= end; j++)
                if (isSys[j]) thread(runProcess, j, true).join();
            for (int j = i; j <= end; j++)
                if (!isSys[j]) thread(runProcess, j, false).join();
        } else {
            for (int j = i; j <= end; j++)
                thread(runProcess, j, false).join();
        }
    }

    cout << "\n=== ALL PROCESSES EXECUTED ===\n";
    return 0;
}


