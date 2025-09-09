#pragma once
#include <thread>
#include <future>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

class ThreadPool {
private:
    std::vector<std::thread> threads;                      // Vector to hold threads
    std::queue<std::function<void()>> tasks;               // Queue to store tasks
    std::mutex queue_mutex;                                // Mutex for synchronizing task queue
    std::condition_variable condition;                     // Condition variable to signal threads
    std::atomic<bool> stop;                                // Flag to stop threads
    std::atomic<int> active_tasks{0};                      // Tracks the number of active tasks
    int max_threads;                                       // Maximum number of threads

public:
    // Constructor
    ThreadPool(int max_threads);

    // Template function to enqueue tasks
    template<class F>
    void run(F&& f) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace(std::forward<F>(f));
        }
        condition.notify_one();
    }

    // Waits for all tasks to finish
    void wait_all();

    // Destructor
    ~ThreadPool();
};
