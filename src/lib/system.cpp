#include "lib/system.h"

#include <libopencm3/cm3/systick.h>

System::ClockResolution System::clock_res_ = System::ClockResolution::kStdRes;

namespace {
volatile uint64_t counter = 0;
}  // namespace

extern "C" void sys_tick_handler();

extern "C" void sys_tick_handler() {
  ++counter;
}

void System::Init(ClockResolution clock_res) {
  clock_res_ = clock_res;

  systick_set_reload(168000000 / clock_res_);
  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
  systick_counter_enable();

  systick_interrupt_enable();
}

uint64_t System::GetUs() {
  switch (clock_res_) {
    case kHighRes:
      return counter;
    case kStdRes:
      return counter * 1000;
    case kLowRes:
    default:
      return counter * 1000000;
  }
}

uint64_t System::GetMs() {
  return GetUs() / 1000;
}

uint64_t System::GetS() {
  return GetUs() / 1000000;
}

void System::DelayUs(uint64_t wait_us) {
  uint64_t wake_tick = System::GetUs() + wait_us;
  while (wake_tick > System::GetUs());
}

void System::DelayMs(uint64_t wait_ms) {
  DelayUs(wait_ms * 1000);
}

void System::DelayS(uint64_t wait_s) {
  DelayUs(wait_s * 1000000);
}
