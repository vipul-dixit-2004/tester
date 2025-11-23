#include <iostream>
#include <thread>
#include <chrono> // For std::chrono::seconds

void worker_function() {
    std::cout << "Worker thread starting..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate work
    std::cout << "Worker thread finished." << std::endl;
}

int main() {
    std::cout << "Main thread starting..." << std::endl;
    std::thread t1(worker_function); // Create a new thread

    // The main thread will block here until t1 finishes execution
    t1.join(); 

    std::cout << "Main thread finished after joining worker thread." << std::endl;
    return 0;
}