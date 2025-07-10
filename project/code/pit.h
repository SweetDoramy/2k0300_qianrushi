#ifndef _PIT_H
#define _PIT_H

#include <thread>
#include <atomic>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <chrono>

struct TimerTask {
    std::chrono::steady_clock::time_point next_trigger;
    std::chrono::milliseconds interval;
    std::function<void()> callback;
    bool operator<(const TimerTask& other) const {
        return next_trigger > other.next_trigger; // 优先队列默认是大顶堆
    }
};

class TimerManager {
private:
    std::thread timer_thread;
    std::atomic<bool> running;
    std::priority_queue<TimerTask> tasks;
    std::mutex mtx;
    std::condition_variable cv;

    void timerLoop();

public:
    TimerManager();
    ~TimerManager();
    void addTimer(std::chrono::milliseconds interval, std::function<void()> callback);
    // 可选：支持移除定时器，暂停/恢复等功能
};

#endif // _PIT_H