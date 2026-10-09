#pragma once
#include <array>
#include <cstdint>
#include <stdexcept>
enum class AirState { Warmup, Normal, Alert, Fault };
struct AirConfig {
  int trip = 1800, reset = 1600;
  std::uint32_t warmupMs = 60000, confirmationMs = 5000, maxGapMs = 3000;
};
struct AirStatus {
  AirState state;
  double average, variance;
  int raw;
  std::uint32_t sequence;
};
class AirMonitor {
  AirConfig config_;
  std::array<int, 32> samples_{};
  std::size_t cursor_ = 0, count_ = 0;
  std::int64_t sum_ = 0, sumSquares_ = 0;
  std::uint32_t boot_ = 0, last_ = 0, above_ = 0, sequence_ = 0;
  AirState state_ = AirState::Warmup;
  bool initialized_ = false, high_ = false;
  int raw_ = 0;
  void change(AirState state) {
    if (state_ != state) {
      state_ = state;
      sequence_++;
    }
  }

public:
  explicit AirMonitor(AirConfig c = {}) : config_(c) {
    if (c.reset < 0 || c.trip > 4095 || c.reset >= c.trip ||
        !c.confirmationMs || !c.maxGapMs ||
        c.warmupMs >= 0x80000000U || c.confirmationMs >= 0x80000000U ||
        c.maxGapMs >= 0x80000000U)
      throw std::invalid_argument("invalid air thresholds");
  }
  AirStatus tick(std::uint32_t now) {
    if (initialized_ && std::uint32_t(now - last_) > config_.maxGapMs)
      change(AirState::Fault);
    return snapshot();
  }
  AirStatus sample(int raw, std::uint32_t now) {
    raw_ = raw;
    if (raw < 1 || raw > 4094 ||
        (initialized_ && std::uint32_t(now - last_) > config_.maxGapMs)) {
      change(AirState::Fault);
      return snapshot();
    }
    if (!initialized_) {
      boot_ = now;
      initialized_ = true;
    }
    last_ = now;
    if (count_ == samples_.size()) {
      sum_ -= samples_[cursor_];
      sumSquares_ -= std::int64_t(samples_[cursor_]) * samples_[cursor_];
    } else
      count_++;
    samples_[cursor_] = raw;
    cursor_ = (cursor_ + 1) % samples_.size();
    sum_ += raw;
    sumSquares_ += std::int64_t(raw) * raw;
    if (state_ == AirState::Fault)
      return snapshot();
    if (std::uint32_t(now - boot_) < config_.warmupMs)
      return snapshot();
    if (state_ == AirState::Warmup)
      change(AirState::Normal);
    double avg = double(sum_) / count_;
    if (avg >= config_.trip) {
      if (!high_) {
        high_ = true;
        above_ = now;
      }
      if (std::uint32_t(now - above_) >= config_.confirmationMs)
        change(AirState::Alert);
    } else if (avg <= config_.reset) {
      high_ = false;
      change(AirState::Normal);
    }
    return snapshot();
  }
  void reset() {
    samples_ = {};
    cursor_ = count_ = 0;
    sum_ = sumSquares_ = 0;
    initialized_ = high_ = false;
    change(AirState::Warmup);
  }
  AirStatus snapshot() const {
    double avg = count_ ? double(sum_) / count_ : 0;
    double variance = count_ ? double(sumSquares_) / count_ - avg * avg : 0;
    return {state_, avg, variance < 0 ? 0 : variance, raw_, sequence_};
  }
};
