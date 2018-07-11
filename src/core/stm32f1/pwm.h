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

#ifndef RTLIB_CORE_STM32F1_PWM_H_
#define RTLIB_CORE_STM32F1_PWM_H_

#if defined(STM32F1)

#include <array>
#include <utility>
#include <libopencm3/stm32/rcc.h>

#include "core/stm32f1/gpio.h"
#include "core/stm32f1/timer.h"

namespace rtlib::core::stm32f1 {

class PWM final : private Timer {
 public:
  enum struct Channel {
    CH1 = TIM_OC1,
    CH2 = TIM_OC2,
    CH3 = TIM_OC3,
    CH4 = TIM_OC4
  };

  using Output = std::pair<Timer::Source, Channel>;

  struct Config {
    Output output;
    Pinout pinout;
    uint16_t prescaler;
    uint16_t period;
  };

  explicit PWM(const Config& config);

  using Timer::Start;
  using Timer::Stop;
  using Timer::SetState;

  void SetValue(std::uint32_t value);

 private:
  const std::uint32_t kMaxPeriod;

  tim_oc_id channel_;
  GPIO gpio_;
};

}  // namespace rtlib::core::stm32f1

#endif  // defined(STM32F1)

#endif  // RTLIB_CORE_STM32F1_PWM_H_
