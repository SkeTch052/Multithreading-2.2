#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <mutex>
#include <vector>

const unsigned int BAR_LENGTH = 12;
std::atomic<int> current_thread(1);
std::mutex mut;
std::chrono::time_point<std::chrono::steady_clock> start_time;

void moveCursor(int x, int y) {
    std::cout << "\033[" << y << ";" << x << "H";
}


void visualBar(int i, int N) {
    std::unique_lock<std::mutex> lock(mut);

    int thread_id = current_thread++;
    std::cout << thread_id << "\t" << std::this_thread::get_id() << "\t\n";

    lock.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    for (size_t j = 0; j <= BAR_LENGTH; j++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(150 * (i + 1)));

        lock.lock();

        moveCursor(17+j, thread_id + 1);
        std::cout << char(219);

        lock.unlock();
    }

    lock.lock();

    moveCursor(25, thread_id + 1);
    std::cout << "\t" << std::chrono::duration<double>(std::chrono::steady_clock::now() - start_time).count() << "s";

    lock.unlock();

    moveCursor(0, N + 2);
}

void progressBar(int& N) {

    std::cout << "#\t" << "ID\t" << "Progress Bar\t" << "Time" << std::endl;

    std::vector<std::thread> t;

    for (size_t i = 0; i < N; i++) {
        start_time = std::chrono::steady_clock::now();
        t.emplace_back(visualBar, i, N);
    } 
    for (auto& thread : t) {
        thread.join();
    }
}

int main()
{
    int number_of_threads = 5;
    progressBar(number_of_threads);
    
    return 0;
}