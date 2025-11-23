#include <iostream>
#include <queue>
#include <string>
#include <iomanip>

using namespace std;

struct Process{
    string procId;
    int arrivalTime;
    int burstTime;
};

struct CompareArrival{
    bool operator()(const Process& p1, const Process& p2){
        return p1.arrivalTime > p2.arrivalTime;
    }
};

int main(){
    priority_queue<Process,vector<Process>,CompareArrival> q;

    q.push({"p1", 0, 3});
    q.push({"p2", 6, 1});
    q.push({"p3", 2, 5});
    q.push({"p4", 7, 4});

    int currentTime = 0;
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;
    int totalProcess = 0;

    cout << left;
    cout << setw(10) << "Process" 
         << setw(12) << "Arrival" 
         << setw(12) << "Burst" 
         << setw(14) << "Waiting Time" 
         << setw(17) << "Turnaround Time" << "\n";
    cout << "--------------------------------------------------------------\n";

    while(!q.empty()) {
        Process process = q.top();
        q.pop();
        totalProcess++;

        if(currentTime < process.arrivalTime){
            currentTime = process.arrivalTime;
        }

        int waitingTime = currentTime - process.arrivalTime;
        currentTime += process.burstTime;
        int turnaroundTime = currentTime - process.arrivalTime;

        cout << setw(10) << process.procId
             << setw(12) << process.arrivalTime
             << setw(12) << process.burstTime
             << setw(14) << waitingTime
             << setw(17) << turnaroundTime << "\n";

        totalWaitingTime += waitingTime;
        totalTurnaroundTime += turnaroundTime;
    }

    double avgWaitingTime = static_cast<double>(totalWaitingTime) / totalProcess;
    double avgTurnaroundTime = static_cast<double>(totalTurnaroundTime) / totalProcess;

    cout << "\nAverage Waiting Time: " << avgWaitingTime << "s\n";
    cout << "Average Turnaround Time: " << avgTurnaroundTime << "s\n";

    return 0;
}
