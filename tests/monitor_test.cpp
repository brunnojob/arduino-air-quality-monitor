#include "air_monitor.hpp"
#include <cassert>
int main() {
  AirMonitor m({1800, 1600, 2000, 2000, 3000});
  assert(m.sample(2000, 1000).average == 2000);
  assert(m.sample(2000, 2000).state == AirState::Warmup);
  m.sample(2000, 3000);
  m.sample(2000, 4000);
  assert(m.sample(2000, 5000).state == AirState::Alert);
  assert(m.sample(0, 6000).state == AirState::Fault);
  m.reset();
  assert(m.sample(1000, 7000).state == AirState::Warmup);
  assert(m.sample(1000, 8000).variance == 0);
  assert(m.sample(1000, 9000).state == AirState::Normal);
}
