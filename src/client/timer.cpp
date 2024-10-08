#include "timer.hpp"

#include <thread>

FrameTimer::FrameTimer(int fps)
    : _delta(1.f / (float)fps)
    , _frameDuration(std::chrono::duration_cast<Clock::duration>(
        std::chrono::duration<double>(1.0 / fps)))
    , _start(Clock::now())
    , _lastFrame(0)
{ }

float FrameTimer::delta() const
{
    return _delta;
}

int FrameTimer::operator()()
{
    auto currentFrame = (Clock::now() - _start) / _frameDuration;
    auto framesPassed = static_cast<int>(currentFrame - _lastFrame);
    _lastFrame = currentFrame;
    return framesPassed;
}

void FrameTimer::relax() const
{
    auto wakeTime = _start + (_lastFrame + 1) * _frameDuration;
    std::this_thread::sleep_until(wakeTime);
}
