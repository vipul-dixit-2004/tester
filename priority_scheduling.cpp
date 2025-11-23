#include<iostream>
#include<queue>
#include<string>
#include <iomanip>
#include <algorithm>

using namespace std;
struct Priority_Process_Queue{
    string procId;
    int priority;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    bool isCompleted;

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
int main(){
    priority_queue<Priority_Process_Queue,vector<Priority_Process_Queue>,ComparePriority> pq;
    priority_queue<Priority_Process_Queue,vector<Priority_Process_Queue>,CompareArrival> aq;
    aq.push({"p1",10,0,23,0,false});
    aq.push({"p2",30,2,23,0,false});
    aq.push({"p3",20,3,23,0,false});

    //intially bt = rt

    // while(!pq.empty()){
    //     auto cur = pq.top();
    //     cout << cur.procId <<endl;
    //     pq.pop();
    // }
    int tq = 2;
    int currentTime = 0;
    int totalTurnAroundTime=0;//ct-at
    int totalWaitingTime=0;//tat-bt
    while(!aq.empty()){
        while(!aq.empty() && aq.top().arrivalTime<=currentTime){
            pq.push(aq.top());
            pq.pop();
        }
        //take top from pq
        auto curProcess = pq.top();
        pq.pop();

        if(curProcess.remainingTime<=tq){
            currentTime+=curProcess.remainingTime;
            curProcess.remainingTime=0;
            
        }else{
            currentTime+=tq;
            curProcess.remainingTime-=tq;
            aq.push(curProcess);
        }

    }
}












































