#pragma once

#ifndef BOROV_ENGINE_TIMER_HPP_INCLUDED
#define BOROV_ENGINE_TIMER_HPP_INCLUDED

#include <chrono>

namespace borov_engine {

class Timer {
  public:
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;
    using FrameCount = std::uint16_t;

    explicit Timer() noexcept;
    explicit Timer(std::nullptr_t) noexcept;

    void Reset();
    void Tick();

    [[nodiscard]] float StartTime() const;

    [[nodiscard]] float DeltaTime() const;

    [[nodiscard]] float FramesPerSecond() const;

  private:
    explicit Timer(TimePoint time_point) noexcept;

    [[nodiscard]] float SecondsFrom(TimePoint time_point) const;

    TimePoint start_;
    TimePoint current_;
    TimePoint previous_;
    TimePoint previous_frame_;
    FrameCount frame_count_;
    float frames_per_second_;
};

}

#endif //BOROV_ENGINE_TIMER_HPP_INCLUDED