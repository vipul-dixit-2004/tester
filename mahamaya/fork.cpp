#include <iostream>
#include <unistd.h>    // for fork(), getpid(), getppid()
#include <sys/types.h> // for pid_t
#include <cstdlib>     // for exit()

using namespace std;

// ----------------------------------------------
// Function 1: Single fork demonstration
// ----------------------------------------------
void singleForkExample() {
    pid_t pid;

    cout << "\n--- SINGLE FORK EXAMPLE ---" << endl;
    cout << "Before fork() call" << endl;

    pid = fork();  // Create a new process

    if (pid < 0) {
        cerr << "Fork failed!" << endl;
        return;
    } 
    else if (pid == 0) {
        // Child process
        cout << "This is the child process." << endl;
        cout << "Child PID: " << getpid() << endl;
        cout << "Parent PID: " << getppid() << endl;

        cout << "This line is executed by both parent and child." << endl;

        exit(0); // Child exits here to avoid running next function
    } 
    else {
        // Parent process
        cout << "This is the parent process." << endl;
        cout << "Parent PID: " << getpid() << endl;
        cout << "Child PID: " << pid << endl;
    }

    cout << "This line is executed only by parent after child exits." << endl;
}

// ----------------------------------------------
// Function 2: Multiple fork demonstration
// ----------------------------------------------
void multipleForkExample() {
    cout << "\n--- MULTIPLE FORK EXAMPLE ---" << endl;
    cout << "Program started. PID: " << getpid() << endl;

    // First fork
    pid_t pid1 = fork();

    if (pid1 == 0) {
        // This block runs in the first child process
        cout << "[Child 1] PID: " << getpid() 
             << " | PPID: " << getppid() << endl;
        // Optional: exit(0); if you want child 1 not to create another fork
    } else {
        // This block runs in the parent process
        cout << "[Parent after first fork] PID: " << getpid() 
             << " | Child PID: " << pid1 << endl;
    }

    // Second fork (executed by both parent and child)
    pid_t pid2 = fork();

    if (pid2 == 0) {
        cout << "[New Child from second fork] PID: " << getpid() 
             << " | PPID: " << getppid() << endl;
    } else {
        cout << "[Process " << getpid() << "] created child PID: " 
             << pid2 << endl;
    }

    // A line that runs in every process
    cout << ">>> Process PID " << getpid() 
         << " still running. Parent PID: " << getppid() << endl;
}

// ----------------------------------------------
// Main function: call both examples independently
// ----------------------------------------------
int main() {
    // First, run single fork example
    singleForkExample();

    // Wait a little to separate outputs (optional)
    sleep(2);

    // Then, run multiple fork example independently
    multipleForkExample();

    return 0;
}
