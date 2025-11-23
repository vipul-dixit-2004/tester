#include <iostream>
#include <unistd.h>     
#include <sys/types.h>  
#include <sys/wait.h>   

using namespace std;

int main() {
    cout << "Parent process started. PID: " << getpid() << endl;


    for (int i = 1; i <= 5; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            cerr << "Fork failed for child " << i << endl;
            return 1;
        } 
        else if (pid == 0) {

            cout << "Child " << i << " started. PID: " << getpid()
                 << ", Parent PID: " << getppid() << endl;

            sleep(2 + i); 
            cout << "Child " << i << " finished work." << endl;

            return 0; 
        }

    }

    for (int i = 1; i <= 6; i++) {
        int status;
        pid_t child_pid = wait(&status);

        if (WIFEXITED(status))
            cout << "Parent: Child with PID " << child_pid 
                 << " exited with status " << WEXITSTATUS(status) << endl;
        else
            cout << "Parent: Child with PID " << child_pid 
                 << " terminated abnormally." << endl;
    }

    cout << "Parent process finished after all children." << endl;

    return 0;
}
