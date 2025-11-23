
#include<iostream>
#include<queue>
#include<string>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Priority_Process_Queue {
    string procId;
    int priority;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int completionTime = 0;
    bool isCompleted = false;
};

struct ComparePriority {
    bool operator()(const Priority_Process_Queue& p1, const Priority_Process_Queue& p2) {
        return p1.priority < p2.priority;
    }
};

struct CompareArrival {
    bool operator()(const Priority_Process_Queue& p1, const Priority_Process_Queue& p2) {
        return p1.arrivalTime > p2.arrivalTime;
    }
};

void takeInputPriorityQ(priority_queue<Priority_Process_Queue, vector<Priority_Process_Queue>, CompareArrival>& aq){
    int p;
    cout<<"Enter no of process: ";
    cin>>p;
    while(p--){
        Priority_Process_Queue process;
        cout<<"Enter Process Id: "<<endl;
        cin>>process.procId;
        
        cout<<"\nEnter Priority for process "<< process.procId<<endl;
        cin>>process.priority;
        
        cout<<"\nEnter Arrival Time: "<<endl;
        cin>>process.arrivalTime;

        cout<<"\nEnter busrt Time: "<<endl;
        cin>>process.burstTime;
        process.remainingTime = process.burstTime;
        aq.push(process);
    }
}

int main() {
    priority_queue<Priority_Process_Queue, vector<Priority_Process_Queue>, ComparePriority> pq;
    priority_queue<Priority_Process_Queue, vector<Priority_Process_Queue>, CompareArrival> aq;
    
    takeInputPriorityQ(aq);

    int n=aq.size();
    int tq = 2;
    int currentTime = 0;
    int totalTurnAroundTime = 0;
    int totalWaitingTime = 0;
    int completedCount = 0;
    

    vector<Priority_Process_Queue> finishedProcesses;

    while (completedCount < n) {
        // move all arrived processes to ready queue
        while (!aq.empty() && aq.top().arrivalTime <= currentTime) {
            pq.push(aq.top());
            aq.pop();
        }

        if (pq.empty()) {

            if (!aq.empty()) {
                currentTime = aq.top().arrivalTime;
                continue;
            }
            break;  
        }

        auto curProcess = pq.top();
        pq.pop();


        int execTime = min(tq, curProcess.remainingTime);
        currentTime += execTime;
        curProcess.remainingTime -= execTime;


        if (curProcess.remainingTime == 0) {
            curProcess.completionTime = currentTime;
            finishedProcesses.push_back(curProcess);
            completedCount++;
        } else {
            while (!aq.empty() && aq.top().arrivalTime <= currentTime) {
                pq.push(aq.top());
                aq.pop();
            }
            pq.push(curProcess);
        }
    }
    cout << "Process"<<setw(10)<<"Arrival"<<setw(10)<<"Burst"<<setw(10)<<"Priority"<<setw(10)<<" Completion"<<setw(10)<<" TurnAround"<<setw(10)<<"Waiting\n";
    for (auto &p : finishedProcesses) {
        int tat = p.completionTime - p.arrivalTime;
        int wt = tat - p.burstTime;
        totalTurnAroundTime += tat;
        totalWaitingTime += wt;
        cout << p.procId << setw(10) << p.arrivalTime << setw(11) << p.burstTime << setw(10)
             << p.priority << setw(10) << p.completionTime << setw(10)
             << tat << setw(10) << wt << "\n";
    }

    cout << fixed << setprecision(2);
    cout << "Average Turnaround Time: " << (float)totalTurnAroundTime / n << "\n";
    cout << "Average Waiting Time: " << (float)totalWaitingTime / n << "\n";

    return 0;
}
