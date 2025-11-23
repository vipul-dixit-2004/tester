#include <iostream>
#include <queue>
#include <string>
#include <iomanip>

using namespace std;

struct Process{
    string procId;
    int burstTime;
};

struct CompareArrival{
    bool operator()(const Process& p1, const Process& p2){
        return p1.burstTime > p2.burstTime;
    }
};

int main(){
    priority_queue<Process,vector<Process>,CompareArrival> q;

    q.push({"p1", 5});
    q.push({"p2", 8});
    q.push({"p3", 3});
    q.push({"p4", 2});

    int currentTime = 0;
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;
    int totalProcess = 0;

    cout << left;
    cout << setw(10) << "Process" 
         << setw(12) << "Burst" 
         << setw(17) << "Turnaround Time"
         << setw(14) << "Waiting Time" << "\n";
    cout << "--------------------------------------------------------------\n";

    while(!q.empty()) {
        Process process = q.top();
        q.pop();
        totalProcess++;
        currentTime +=process.burstTime;
    int waitingTime = currentTime-process.burstTime; 
    


        cout << setw(10) << process.procId
             << setw(12) << process.burstTime
             << setw(17) << currentTime 
             << setw(14) << waitingTime << "\n";

             totalTurnaroundTime+=currentTime;
             totalWaitingTime+= waitingTime;

        
    }

    double avgWaitingTime = static_cast<double>(totalWaitingTime) / totalProcess;
    double avgTurnaroundTime = static_cast<double>(totalTurnaroundTime) / totalProcess;

    cout << "\nAverage Waiting Time: " << avgWaitingTime << "s\n";
    cout << "Average Turnaround Time: " << avgTurnaroundTime << "s\n";

    return 0;
}
