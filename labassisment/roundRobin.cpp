#include <iostream>
#include <queue>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Process
{
    int procId;
    int burstTime;
    int arrivalTime;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
};

int main()
{
    int n, timeQuantum;
    cout << "Enter number of processes: ";
    cin >> n;

    cout << "Enter time quantum: ";
    cin >> timeQuantum;

    vector<Process> processes(n);
    queue<int> readyQueue;

    for (int i = 0; i < n; ++i)
    {
        cout << "Enter arrival time and burst time for process " << i << ": ";
        processes[i].procId = i;
        cin >> processes[i].arrivalTime >> processes[i].burstTime;
    }

    vector<int> remainingBurst(n);
    for (int i = 0; i < n; ++i)
    {
        remainingBurst[i] = processes[i].burstTime;
    }

    int currentTime = 0;
    int completed = 0;
    vector<bool> inQueue(n, false);

    for (int i = 0; i < n; ++i)
    {
        if (processes[i].arrivalTime <= currentTime)
        {
            readyQueue.push(i);
            inQueue[i] = true;
        }
    }

    while (!readyQueue.empty())
    {
        int idx = readyQueue.front();
        readyQueue.pop();

        int execTime = min(timeQuantum, remainingBurst[idx]);
        currentTime += execTime;
        remainingBurst[idx] -= execTime;

        for (int i = 0; i < n; ++i)
        {
            if (!inQueue[i] && processes[i].arrivalTime <= currentTime)
            {
                readyQueue.push(i);
                inQueue[i] = true;
            }
        }

        if (remainingBurst[idx] > 0)
        {

            readyQueue.push(idx);
        }
        else
        {

            processes[idx].completionTime = currentTime;
            processes[idx].turnaroundTime = currentTime - processes[idx].arrivalTime;
            processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
            completed++;
        }
    }
    double totalTurnAroundTime = 0;
    double totalWaitingTime = 0;
    // Order output by ascending completion time (earliest finishing first)
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b){
        return a.completionTime < b.completionTime; // smaller completionTime comes first
    });

    cout << "Process" << setw(10) << "Arrival" << setw(10) << "Burst" << setw(12) << "Completion " << setw(12) << " Turnaround " << setw(12) << "Waiting " << endl;
    for (int i = 0; i < n; ++i)
    {
        cout << processes[i].procId << setw(10) << processes[i].arrivalTime << setw(12)
             << processes[i].burstTime << setw(10) << processes[i].completionTime << setw(12)
             << processes[i].turnaroundTime << setw(12) << processes[i].waitingTime << "\n";

        totalTurnAroundTime += processes[i].turnaroundTime;
        totalWaitingTime += processes[i].waitingTime;
    }
    cout << "Turn Around Time: " << static_cast<double>(totalTurnAroundTime / n) << endl;
    cout << "Waiting Time: " << static_cast<double>(totalWaitingTime / n) << endl;

    return 0;
}
