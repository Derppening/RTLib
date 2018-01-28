/*
 * This file is part of RTLib.
 *
 * Copyright (C) 2017-2018 Derppening <david.18.19.21@gmail.com>
 *
 * RTLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RTLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with RTLib.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "lib/system.h"

#include <libopencm3/cm3/systick.h>

extern "C" void sys_tick_handler();

namespace {

volatile uint64_t counter = 0;

}  // namespace

extern "C" void sys_tick_handler() {
  ++counter;
}

namespace rtlib::lib {

bool System::has_init_ = false;
System::ClockResolution System::clock_res_ = System::ClockResolution::kStdRes;

void System::Init(ClockResolution clock_res) {
  // check whether clock has already been initialized
  if (has_init_) { return; }

  clock_res_ = clock_res;

#if defined(STM32F1)
  systick_set_reload(72000000 / clock_res_);
#elif defined(STM32F4)
  systick_set_reload(168000000 / clock_res_);
#endif
  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
  systick_counter_enable();

  systick_interrupt_enable();

  has_init_ = true;
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

}  // namespace rtlib::lib
