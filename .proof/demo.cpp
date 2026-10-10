#include "air_monitor.hpp"
#include <cassert>
#include <iostream>
int main() {
 AirMonitor monitor({1800,1600,1,100,3000});
 monitor.sample(1000,100);
 assert(monitor.tick(3101).state==AirState::Fault);
 assert(monitor.sample(1000,3102).state==AirState::Fault);
 monitor.reset();
 assert(monitor.sample(1000,3103).state==AirState::Warmup);
 bool rejected=false;
 try { AirMonitor invalid({1800,1600,0,0x80000000U,3000}); } catch(...) { rejected=true; }
 assert(rejected);
std::cout << "stale air sensor latched; explicit reset restores warmup\n";
}
