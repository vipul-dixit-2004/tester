#include <iostream>
#include <unistd.h>   
#include <sys/types.h> 

using namespace std;

int main() {
    cout << "Before fork()" << endl;

    pid_t pid = fork();  

    if (pid < 0) {
        cerr << "Fork failed!" << endl;
        return 1;
    } 
    else if (pid == 0) {

        cout << "Hello from CHILD process!" << endl;
        cout << "Child PID: " << getpid() << ", Parent PID: " << getppid() << endl;
    } 
    else {

        cout << "Hello from PARENT process!" << endl;
        cout << "Parent PID: " << getpid() << ", Child PID: " << pid << endl;
    }

    cout << "This line is executed by BOTH parent and child!" << endl;

    return 0;
}
