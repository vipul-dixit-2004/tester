#include <iostream>
#include <unistd.h>    
#include <sys/wait.h>  

int main() {
    pid_t pid = fork();

    if (pid < 0) {

        std::cerr << "Fork failed!\n";
        return 1;
    }
    else if (pid == 0) {

        std::cout << "Child process (PID: " << getpid() << ")\n";


        execl("/bin/ls", "ls", "-l", (char *)NULL);

        std::cerr << "exec() failed!\n";
        return 1;
    }
    else {

        std::cout << "Parent process (PID: " << getpid() << ") waiting for child...\n";

        int status;
        waitpid(pid, &status, 0);

        std::cout << "Child finished with status " << status << "\n";
    }

    return 0;
}
