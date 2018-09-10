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

#include "core/stm32f4/pwm.h"

#if defined(STM32F4)

#include <algorithm>

#include "core/assert.h"
#include "util/ranges.h"

namespace rtlib::core::stm32f4 {

PWM::PWM(const Config& config) :
    Timer(config.output.first,
          Timer::ClockDivisionRatio::kUnchanged,
          Timer::CenterAlignedMode::kEdgeAligned,
          Timer::Direction::kUp,
          config.prescaler,
          config.period,
          0), kMaxPeriod(config.period), channel_(tim_oc_id(config.output.second)) {
  GPIO::Config gpio_config;
  gpio_config.pin = config.pinout;
  gpio_config.mode = GPIO::Mode::kAF;
  gpio_config.pullup = GPIO::Pullup::kNone;
  gpio_config.speed = GPIO::Speed::k50MHz;
  gpio_config.altfn = GetAltFn();
  gpio_ = GPIO(gpio_config);

  timer_disable_oc_output(timer_, channel_);
  timer_set_oc_mode(timer_, channel_, TIM_OCM_PWM1);
  timer_set_oc_value(timer_, channel_, 0);
  timer_enable_oc_output(timer_, channel_);
}

void PWM::SetValue(std::uint32_t value) {
  using libutil::in_range;

#if defined(RTLIB_ENABLE_VALIDATION)
  Assert(in_range<std::uint32_t>(value, 0, kMaxPeriod), __FILE__, __LINE__, __func__);
#endif  // defined(RTLIB_ENABLE_VALIDATION)

  timer_set_oc_value(timer_, channel_, value);
}

std::uint8_t PWM::GetAltFn() const {
  switch (timer_) {
    case TIM1:
    case TIM2:
      return GPIO_AF1;
    case TIM3:
    case TIM4:
    case TIM5:
      return GPIO_AF2;
    case TIM6:
    case TIM7:
      // no altfn
      return GPIO_AF0;
    case TIM8:
    case TIM9:
    case TIM10:
    case TIM11:
      return GPIO_AF3;
    case TIM12:
    case TIM13:
    case TIM14:
      return GPIO_AF9;
    default:
#if defined(RTLIB_ENABLE_VALIDATION)
      Assert(false, __FILE__, __LINE__, __func__);
#endif  // defined(RTLIB_ENABLE_VALIDATION)
      return GPIO_AF0;
  }
}

}  // namespace rtlib::core::stm32f4

#endif  // defined(STM32F4)