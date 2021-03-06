#pragma once

#include <queue>
#include <condition_variable>

template <typename T>
class ThreadedQueue {
public:
    void pop(T & t) {
        std::unique_lock<std::mutex> lock(mutex);
        while (queue.empty()) {
            cond.wait(lock);
        }
        t = queue.front();
        queue.pop();
    }

    void push(const T & t) {
        {
            std::unique_lock<std::mutex> lock(mutex);
            queue.push(t);
        }
        cond.notify_one();
    }

private:
    std::queue<T> queue;
    std::mutex mutex;
    std::condition_variable cond;
};
