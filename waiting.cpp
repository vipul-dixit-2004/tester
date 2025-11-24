#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main()
{
    cout << "Before fork()" << endl;

    pid_t pid = fork();

    if (pid < 0)
    {
        cerr << "Fork failed!" << endl;
        return 1;
    }
    else if (pid == 0)
    {

        cout << "Child: Starting work..." << endl;
        sleep(2);
        cout << "Child: Finished work!" << endl;
        cout << "Child PID: " << getpid() << ", Parent PID: " << getppid() << endl;
    }
    else
    {

        cout << "Parent: Waiting for child to finish..." << endl;

        int status;
        wait(&status);

        cout << "Parent: Child finished!" << endl;
        cout << "Parent PID: " << getpid() << ", Child PID: " << pid << endl;

        if (WIFEXITED(status))
            cout << "Parent: Child exited normally with code " << WEXITSTATUS(status) << endl;
    }

    cout << "This line is executed by BOTH parent and child!" << endl;

    return 0;
}
