#include "thread_pool.hpp"

ThreadPool::ThreadPool(int max_threads) : max_threads(max_threads), stop(false) {
    for (int i = 0; i < max_threads; ++i) {
        threads.emplace_back([this, i] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queue_mutex);
                    condition.wait(lock, [this] { return stop || !tasks.empty(); });
                    if (stop && tasks.empty()) {
                        return;
                    }
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                active_tasks++;
                task();
                active_tasks--;
                {
                    std::lock_guard<std::mutex> lock(queue_mutex);
                    condition.notify_all();
                }
            }
        });
    }
}


void ThreadPool::wait_all() {
    std::unique_lock<std::mutex> lock(queue_mutex);
    while (true) {
        if (tasks.empty() && active_tasks == 0) break;
        condition.wait(lock);
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all(); 
    for (std::thread& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}
