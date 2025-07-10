#ifndef DOUBLEBUFFER_H
#define DOUBLEBUFFER_H

#include <atomic>
#include <mutex>

// 高频数据结构
struct HighFreqData {
    float gyroz = 0;
    float yaw = 0;
    float img_mid_value = 0;
    int encoder_L = 0;
    int encoder_R = 0;

    // 根据需要添加其他高频数据成员
};

// 低频数据结构
struct LowFreqData {
    bool keyShortPressed_13;
    bool keyShortPressed_14;
    bool keyShortPressed_15;
    bool keyShortPressed_16;

    bool keyLongPressed_13;
    bool keyLongPressed_14;
    bool keyLongPressed_15;
    bool keyLongPressed_16;
    int battery_vol = 0;
    // 根据需要添加其他低频数据成员
};

struct ImageData
{
    cv::Mat img; // 存储图像数据
};


// 模板化双缓冲区类
template <typename T>
class DoubleBuffer {
public:
    DoubleBuffer() : currentBuffer(0) {}

    // 写入数据到缓冲区
    void write(const T& data) {
        std::lock_guard<std::mutex> lock(mtx);
        buffers[1 - currentBuffer].data = data;
        currentBuffer.store(1 - currentBuffer.load(), std::memory_order_release);
    }

    // 从缓冲区读取数据
    T read() const {
        std::lock_guard<std::mutex> lock(mtx);
        int bufferIndex = currentBuffer.load(std::memory_order_acquire);
        return buffers[bufferIndex].data;
    }

private:
    struct Buffer {
        T data;
    };

    Buffer buffers[2];
    std::atomic<int> currentBuffer;
    mutable std::mutex mtx;
};

#endif // DOUBLEBUFFER_H  