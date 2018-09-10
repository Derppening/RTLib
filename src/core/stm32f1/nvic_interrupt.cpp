/*
 * This file is part of RTLib.
 *
 * Copyright (C) 2018 Derppening <david.18.19.21@gmail.com>
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

#include "core/stm32f1/nvic_interrupt.h"

#if defined(STM32F1)

#include "core/assert.h"

namespace rtlib::core::stm32f1 {
NVICInterrupt::~NVICInterrupt() {
#if defined(DTOR_RESET_PERIPH)
  if (IsBinded()) {
    nvic_clear_pending_irq(nvic_);
    Disable();
  }
#endif  // defined(DTOR_RESET_PERIPH)
}

void NVICInterrupt::Enable() const {
#if defined(RTLIB_ENABLE_VALIDATION)
  Assert(IsBinded(), __FILE__, __LINE__, __func__);
#endif  // defined(RTLIB_ENABLE_VALIDATION)

  nvic_enable_irq(nvic_);
}

void NVICInterrupt::Disable() const {
#if defined(RTLIB_ENABLE_VALIDATION)
  Assert(IsBinded(), __FILE__, __LINE__, __func__);
#endif  // defined(RTLIB_ENABLE_VALIDATION)

  if (IsBinded()) {
    nvic_disable_irq(nvic_);
  }
}

void NVICInterrupt::SetPriority(uint8_t priority) const {
#if defined(RTLIB_ENABLE_VALIDATION)
  Assert(IsBinded(), __FILE__, __LINE__, __func__);
#endif  // defined(RTLIB_ENABLE_VALIDATION)

  if (IsBinded()) {
    nvic_set_priority(nvic_, priority);
  }
}

bool NVICInterrupt::HasPending() const {
  if (IsBinded()) {
    return bool(nvic_get_pending_irq(nvic_));
  }

  return false;

}
}  // namespace rtlib::core::stm32f1

#endif  // defined(STM32F1)
