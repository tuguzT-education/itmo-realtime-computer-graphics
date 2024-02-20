#include "borov_engine/timer.hpp"

namespace borov_engine {

Timer::Timer() noexcept: Timer{Clock::now()} {}

Timer::Timer(std::nullptr_t) noexcept: Timer{TimePoint{}} {}

Timer::Timer(TimePoint time_point) noexcept
    : start_{time_point},
      current_{start_},
      previous_{current_},
      previous_frame_{previous_},
      frame_count_{},
      frames_per_second_{} {}

void Timer::Reset() {
    *this = Timer{};
}

void Timer::Tick() {
    previous_ = current_;
    current_ = Clock::now();

    frame_count_ += 1;
    auto second = std::chrono::seconds{1};
    auto time_from_previous_frame = current_ - previous_frame_;
    if (time_from_previous_frame > second) {
        frames_per_second_ = static_cast<float>(frame_count_) / SecondsFrom(time_from_previous_frame);
        previous_frame_ += second;
        frame_count_ = 0;
    }
}

float Timer::StartTime() const {
    return SecondsFrom(start_);
}

float Timer::DeltaTime() const {
    return SecondsFrom(previous_);
}

float Timer::FramesPerSecond() const {
    return frames_per_second_;
}

float Timer::SecondsFrom(TimePoint time_point) const {
    return SecondsFrom(current_ - time_point);
}

float Timer::SecondsFrom(Duration duration) {
    using std::chrono::microseconds;

    auto ticks = std::chrono::duration_cast<microseconds>(duration).count();
    return static_cast<float>(ticks) / microseconds::period::den;
}

}
