#pragma once

#include <thread>
#include <atomic>
#include <chrono>

template <typename T>
class TimerInterface {
public:
    virtual void start(const T & t) = 0;
    virtual void stop() = 0;
    virtual bool is_running() const = 0;

    virtual void callback() = 0;
};

class Timer : public TimerInterface<std::chrono::milliseconds> {
public:
    Timer()
            : run(false) {
    }

    ~Timer() {
        stop();
    }

    void start(const std::chrono::milliseconds & interval) override {
        stop();
        run = true;
        timer_thread = std::thread([this, interval] {
            while (run) {
                callback();
                std::this_thread::sleep_for(interval);
            }
        });
    }

    void stop() override {
        if (timer_thread.joinable()) {
            run = false;
            timer_thread.join();
        }
    }

    bool is_running() const override {
        return run;
    }

private:
    std::atomic<bool> run;
    std::thread timer_thread;
};
