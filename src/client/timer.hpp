#pragma once

#include <chrono>

class FrameTimer {
public:
    explicit FrameTimer(int fps);

    float delta() const;
    int operator()();
    void relax() const;

private:
    using Clock = std::chrono::high_resolution_clock;

    float _delta = 0;
    Clock::duration _frameDuration;
    Clock::time_point _start;
    size_t _lastFrame = 0;
};