#include<iostream>
#include<queue>
#include<string>

using namespace std;

struct Process{
    string procId;
    int burstTime; 
};

int main(){
    queue<Process>q;
    int totalWaitingTime=0;
    int totalProcess=0;
    int currentTime=0;
    int totalTurnaroundTime=0;

    q.push({"p1",3});
    q.push({"p2",1});
    q.push({"p3",5});
    q.push({"p4",4});

    cout<<"Processing order and turnaround times:\n";

    while(!q.empty()){
        Process process=q.front();
        q.pop();
        totalProcess++;

        int waitingTime=currentTime;
        currentTime+=process.burstTime;
        int turnaroundTime=currentTime;

        cout<<"Process: "<<process.procId
            <<", Burst Time: "<<process.burstTime
            <<", Waiting Time: "<<waitingTime<<"s"
            <<", Turnaround Time: "<<turnaroundTime<<"s"<<endl;

        totalTurnaroundTime+=turnaroundTime;
        totalWaitingTime+=waitingTime;
    }

    double avgWaitingTime= static_cast<double>(totalWaitingTime)/totalProcess;
    double avgTurnaroundTime= static_cast<double>(totalTurnaroundTime)/totalProcess;

    cout<<"\nAverage Waiting Time: "<<avgWaitingTime<<"s"<<endl;
    cout<<"Average Turnaround Time: "<<avgTurnaroundTime<<"s"<<endl;

    return 0;
}
