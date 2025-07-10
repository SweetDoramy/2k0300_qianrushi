#include "pit.h"

TimerManager::TimerManager() : running(true), timer_thread(&TimerManager::timerLoop, this) {}

TimerManager::~TimerManager() 
{
    running = false;
    cv.notify_all();
    if (timer_thread.joinable()) {
        timer_thread.join();
    }
}

void TimerManager::addTimer(std::chrono::milliseconds interval, std::function<void()> callback)
{
    std::lock_guard<std::mutex> lock(mtx);
    TimerTask task;
    task.interval = interval;
    task.callback = callback;
    task.next_trigger = std::chrono::steady_clock::now() + interval;
    tasks.push(task);
    cv.notify_all();
}

void TimerManager::timerLoop() {
    while (running) {
        std::unique_lock<std::mutex> lock(mtx);
        if (tasks.empty()) {
            cv.wait(lock, [this]() { return !running || !tasks.empty(); });
        } else {
            auto now = std::chrono::steady_clock::now();
            TimerTask next_task = tasks.top();
            if (now >= next_task.next_trigger) {
                tasks.pop();
                lock.unlock();
                // 执行回调
                next_task.callback();
                lock.lock();
                // 重新调度
                next_task.next_trigger += next_task.interval;
                tasks.push(next_task);
            } else {
                cv.wait_until(lock, next_task.next_trigger);
            }
        }
    }
}