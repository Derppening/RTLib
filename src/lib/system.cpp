#include "lib/system.h"

#include <libopencm3/cm3/systick.h>

namespace {
volatile uint32_t counter = 0;
}  // namespace

extern "C" void sys_tick_handler() {
  ++counter;
}

void System::Init() {
  systick_set_reload(168000);
  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
  systick_counter_enable();

  systick_interrupt_enable();
}

uint32_t System::GetMs() {
  return counter;
}
