#include <iostream>
#include <queue>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;
struct Priority_Process_Queue
{
    string procId;
    int priority;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    bool isCompleted;
    int completionTime;
};
struct ComparePriority
{
    bool operator()(const Priority_Process_Queue &p1, const Priority_Process_Queue &p2)
    {
        return p1.priority < p2.priority;
    }
};
struct CompareArrival
{
    bool operator()(const Priority_Process_Queue &p1, const Priority_Process_Queue &p2)
    {
        return p1.arrivalTime > p2.arrivalTime;
    }
};
int main()
{
    priority_queue<Priority_Process_Queue, vector<Priority_Process_Queue>, ComparePriority> pq;
    priority_queue<Priority_Process_Queue, vector<Priority_Process_Queue>, CompareArrival> aq;
    aq.push({"p1", 10, 0, 23, 23, false, 0});
    aq.push({"p2", 30, 2, 23, 23, false, 0});
    aq.push({"p3", 20, 3, 23, 23, false, 0});

    // intially bt = rt

    // while(!pq.empty()){
    //     auto cur = pq.top();
    //     cout << cur.procId <<endl;
    //     pq.pop();
    // }
    int tq = 2;
    int currentTime = 0;
    int totalTurnAroundTime = 0; // ct-at
    int totalWaitingTime = 0;    // tat-bt
    vector<Priority_Process_Queue> finished;
    while (!aq.empty() || !pq.empty())
    {
        // Move newly arrived processes into ready (priority) queue
        while (!aq.empty() && aq.top().arrivalTime <= currentTime)
        {
            auto proc = aq.top();
            aq.pop();
            // ensure remainingTime initialized
            if (proc.remainingTime == 0)
                proc.remainingTime = proc.burstTime;
            pq.push(proc);
        }

        // If no ready process, jump time to next arrival
        if (pq.empty())
        {
            if (!aq.empty())
            {
                currentTime = max(currentTime, aq.top().arrivalTime);
                continue; // loop will move it in next iteration
            }
            else
            {
                break; // all done
            }
        }

        // Take highest priority (larger number if comparator defines so) process
        auto curProcess = pq.top();
        pq.pop();

        int runSlice = min(tq, curProcess.remainingTime);
        currentTime += runSlice;
        curProcess.remainingTime -= runSlice;

        // After running, bring in any processes that arrived during the slice
        while (!aq.empty() && aq.top().arrivalTime <= currentTime)
        {
            auto proc = aq.top();
            aq.pop();
            if (proc.remainingTime == 0)
                proc.remainingTime = proc.burstTime;
            pq.push(proc);
        }

        if (curProcess.remainingTime == 0)
        {
            curProcess.isCompleted = true;
            curProcess.completionTime = currentTime;
            finished.push_back(curProcess);
        }
        else
        {
            // Preempted, push back into ready queue
            pq.push(curProcess);
        }
    }

    // Output results
    double totalTurnAroundTime = 0.0;
    double totalWaitingTime = 0.0;
    cout << left << setw(6) << "PID" << setw(10) << "Priority" << setw(8) << "AT" << setw(8) << "BT" << setw(8) << "CT" << setw(8) << "TAT" << setw(8) << "WT" << "\n";
    for (const auto &p : finished)
    {
        int tat = p.completionTime - p.arrivalTime;
        int wt = tat - p.burstTime;
        totalTurnAroundTime += tat;
        totalWaitingTime += wt;
        cout << left << setw(6) << p.procId << setw(10) << p.priority << setw(8) << p.arrivalTime << setw(8) << p.burstTime << setw(8) << p.completionTime << setw(8) << tat << setw(8) << wt << "\n";
    }
    if (!finished.empty())
    {
        cout << "Average TAT: " << (totalTurnAroundTime / finished.size()) << "\n";
        cout << "Average WT: " << (totalWaitingTime / finished.size()) << "\n";
    }
}
