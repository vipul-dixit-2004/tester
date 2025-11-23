#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Process {
    string procId;
    int arrivalTime;
    int burstTime;
    int startTime = -1; 
};

struct CompareArrival {
    bool operator()(const Process& p1, const Process& p2) {
        return p1.arrivalTime > p2.arrivalTime;
    }
};

struct CompareBurst {
    bool operator()(const Process& p1, const Process& p2) {

        if (p1.burstTime == p2.burstTime)
            return p1.arrivalTime > p2.arrivalTime; 
        return p1.burstTime > p2.burstTime;
    }
};

int main() {

    vector<Process> processes = {
        {"p1", 1, 3},
        {"p2", 2, 2},
        {"p3", 0, 3}
    };

    priority_queue<Process, vector<Process>, CompareArrival> arrivalQueue;
    for (auto& p : processes)
        arrivalQueue.push(p);

    priority_queue<Process, vector<Process>, CompareBurst> readyQueue;

    int currentTime = 0;
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;
    int totalProcess = processes.size();

    cout << left;
    cout << setw(10) << "Process"
         << setw(12) << "Arrival"
         << setw(12) << "Burst"
         << setw(17) << "Turnaround Time"
         << setw(14) << "Waiting Time" << "\n";
    cout << "--------------------------------------------------------------\n";

    while (!arrivalQueue.empty() || !readyQueue.empty()) {

        while (!arrivalQueue.empty() && arrivalQueue.top().arrivalTime <= currentTime) {
            readyQueue.push(arrivalQueue.top());
            arrivalQueue.pop();
        }

        if (readyQueue.empty()) {

            currentTime = arrivalQueue.top().arrivalTime;
            continue;
        }


        Process process = readyQueue.top();
        readyQueue.pop();

        if (process.arrivalTime > currentTime) {
            currentTime = process.arrivalTime;
        }

        int startTime = currentTime;
        currentTime += process.burstTime;

        int turnaroundTime = currentTime - process.arrivalTime;
        int waitingTime = turnaroundTime - process.burstTime;

        cout << setw(10) << process.procId
             << setw(12) << process.arrivalTime
             << setw(12) << process.burstTime
             << setw(17) << turnaroundTime
             << setw(14) << waitingTime << "\n";

        totalTurnaroundTime += turnaroundTime;
        totalWaitingTime += waitingTime;
    }

    double avgWaitingTime = static_cast<double>(totalWaitingTime) / totalProcess;
    double avgTurnaroundTime = static_cast<double>(totalTurnaroundTime) / totalProcess;

    cout << "\nAverage Waiting Time: " << avgWaitingTime << "s\n";
    cout << "Average Turnaround Time: " << avgTurnaroundTime << "s\n";

    return 0;
}
