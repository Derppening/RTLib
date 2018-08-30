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

#include "core/stm32f1/timer.h"

#if defined(STM32F1)

#include <array>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>

#include "core/assert.h"
#include "util/ranges.h"

namespace {
std::array<rtlib::core::stm32f1::Timer::HandlerFn, 8> interrupt_fn{nullptr};

extern "C" void tim1_isr() {
  if (interrupt_fn[0] != nullptr) {
    interrupt_fn[0]();
  }

  TIM_SR(TIM1) &= ~uint32_t(TIM_SR_UIF);
}

extern "C" void tim2_isr() {
  if (interrupt_fn[1] != nullptr) {
    interrupt_fn[1]();
  }

  TIM_SR(TIM2) &= ~uint32_t(TIM_SR_UIF);
}

extern "C" void tim3_isr() {
  if (interrupt_fn[2] != nullptr) {
    interrupt_fn[2]();
  }

  TIM_SR(TIM3) &= ~uint32_t(TIM_SR_UIF);
}

extern "C" void tim4_isr() {
  if (interrupt_fn[3] != nullptr) {
    interrupt_fn[3]();
  }

  TIM_SR(TIM4) &= ~uint32_t(TIM_SR_UIF);
}

extern "C" void tim5_isr() {
  if (interrupt_fn[4] != nullptr) {
    interrupt_fn[4]();
  }

  TIM_SR(TIM5) &= ~uint32_t(TIM_SR_UIF);
}

extern "C" void tim6_isr() {
  if (interrupt_fn[5] != nullptr) {
    interrupt_fn[5]();
  }

  TIM_SR(TIM6) &= ~uint32_t(TIM_SR_UIF);
}

extern "C" void tim7_isr() {
  if (interrupt_fn[6] != nullptr) {
    interrupt_fn[6]();
  }

  TIM_SR(TIM7) &= ~uint32_t(TIM_SR_UIF);
}

extern "C" void tim8_isr() {
  if (interrupt_fn[7] != nullptr) {
    interrupt_fn[7]();
  }

  TIM_SR(TIM8) &= ~uint32_t(TIM_SR_UIF);
}
}  // namespace

namespace rtlib::core::stm32f1 {

Timer::Timer(const Config& config) :
    Timer(config.timer,
          config.clock_div,
          config.alignment,
          config.direction,
          config.prescaler,
          config.period,
          config.start_count) {}

Timer::Timer(Source timer,
             ClockDivisionRatio clock_div,
             CenterAlignedMode alignment,
             Direction direction,
             uint32_t prescaler,
             uint32_t period,
             uint32_t start_count) :
    timer_(uint32_t(timer)) {
  InitRcc();

  timer_set_mode(timer_, uint32_t(clock_div), uint32_t(alignment), uint32_t(direction));
  SetPrescaler(prescaler);
  SetPeriod(period);
  SetCounterValue(start_count);
}

Timer::~Timer() {
  SetState(false);

  // TODO(Derppening): Move to Reset() function
  switch (timer_) {
    case TIM1:
      rcc_periph_reset_pulse(RST_TIM1);
      break;
    case TIM2:
    case TIM3:
    case TIM4:
    case TIM5:
    case TIM6:
    case TIM7:
      rcc_periph_reset_pulse(rcc_periph_rst(_REG_BIT(0x10, (timer_ - TIM2_BASE) / 0x0400)));
      break;
    case TIM8:
      rcc_periph_reset_pulse(RST_TIM8);
      break;
    case TIM9:
    case TIM10:
    case TIM11:
      rcc_periph_reset_pulse(rcc_periph_rst(_REG_BIT(0x0c, 19 + (timer_ - TIM9_BASE) / 0x0400)));
      break;
    case TIM12:
    case TIM13:
    case TIM14:
      rcc_periph_reset_pulse(rcc_periph_rst(_REG_BIT(0x10, 6 + (timer_ - TIM12_BASE) / 0x0400)));
      break;
    case TIM15:
    case TIM16:
    case TIM17:
      rcc_periph_reset_pulse(rcc_periph_rst(_REG_BIT(0x0c, 16 + (timer_ - TIM15_BASE) / 0x0400)));
      break;
    default:
      Assert(false, __FILE__, __LINE__, __func__, "Invalid or unsupported Timer");
      break;
  }
}

void Timer::SetCounterValue(uint32_t count) const {
  using libutil::in_range;

  Assert(in_range(count,
                  uint32_t(std::numeric_limits<uint16_t>::min()),
                  uint32_t(std::numeric_limits<uint16_t>::max())), __FILE__, __LINE__, __func__);

  timer_set_counter(timer_, count);
}

void Timer::SetPeriod(uint32_t period) const {
  using libutil::in_range;

  Assert(in_range(period,
                  uint32_t(std::numeric_limits<uint16_t>::min()),
                  uint32_t(std::numeric_limits<uint16_t>::max())), __FILE__, __LINE__, __func__);

  timer_set_period(timer_, period);
}

void Timer::SetPrescaler(uint32_t prescaler) const {
  using libutil::in_range;

  Assert(in_range(prescaler,
                  uint32_t(std::numeric_limits<uint16_t>::min()),
                  uint32_t(std::numeric_limits<uint16_t>::max())), __FILE__, __LINE__, __func__);

  timer_set_prescaler(timer_, prescaler);
}

void Timer::SetReptitionCounter(uint32_t value) const {
  using libutil::in_range;

  Assert(in_range(value,
                  uint32_t(std::numeric_limits<uint16_t>::min()),
                  uint32_t(std::numeric_limits<uint16_t>::max())), __FILE__, __LINE__, __func__);

  timer_set_repetition_counter(timer_, value);
}

void Timer::SetPreload(bool state) const {
  if (state) {
    timer_enable_preload(timer_);
  } else {
    timer_disable_preload(timer_);
  }
}

void Timer::StopAtUpdateEvent(bool state) const {
  if (state) {
    timer_one_shot_mode(timer_);
  } else {
    timer_continuous_mode(timer_);
  }
}

void Timer::SetState(bool state) const {
  if (state) {
    timer_enable_counter(timer_);
  } else {
    timer_disable_counter(timer_);
  }
}

void Timer::SetEnableInterrupt(const Interrupt& interrupt) {
  if (interrupt.fn == nullptr) {
    return;
  }

  switch (timer_) {
    case TIM1:
      switch (interrupt.type) {
        case Interrupt::Type::kBreak:
          nvic_ = NVICInterrupt(NVICInterrupt::kTIM1Break);
          break;
        case Interrupt::Type::kUpdate:
          nvic_ = NVICInterrupt(NVICInterrupt::kTIM1Update);
          break;
        case Interrupt::Type::kTriggerAndCommutation:
          nvic_ = NVICInterrupt(NVICInterrupt::kTIM1TriggerAndCommutation);
          break;
        case Interrupt::Type::kCaptureCompare:
          nvic_ = NVICInterrupt(NVICInterrupt::kTIM1CaptureCompare);
          break;
        default:
          Assert(false, __FILE__, __LINE__, __func__);
          break;
      }
      interrupt_fn[0] = interrupt.fn;
      break;
    case TIM2:
      nvic_ = NVICInterrupt(NVICInterrupt::kTIM2);
      interrupt_fn[1] = interrupt.fn;
      break;
    case TIM3:
      nvic_ = NVICInterrupt(NVICInterrupt::kTIM3);
      interrupt_fn[2] = interrupt.fn;
      break;
    case TIM4:
      nvic_ = NVICInterrupt(NVICInterrupt::kTIM4);
      interrupt_fn[3] = interrupt.fn;
      break;
    case TIM5:
      nvic_ = NVICInterrupt(NVICInterrupt::kTIM5);
      interrupt_fn[4] = interrupt.fn;
      break;
    case TIM6:
      nvic_ = NVICInterrupt(NVICInterrupt::kTIM6);
      interrupt_fn[5] = interrupt.fn;
      break;
    case TIM7:
      nvic_ = NVICInterrupt(NVICInterrupt::kTIM7);
      interrupt_fn[6] = interrupt.fn;
      break;
    case TIM8:
      switch (interrupt.type) {
        case Interrupt::Type::kBreak:
          nvic_ = NVICInterrupt(NVICInterrupt::kTIM8Break);
          break;
        case Interrupt::Type::kUpdate:
          nvic_ = NVICInterrupt(NVICInterrupt::kTIM8Update);
          break;
        case Interrupt::Type::kTriggerAndCommutation:
          nvic_ = NVICInterrupt(NVICInterrupt::kTIM8TriggerAndCommutation);
          break;
        case Interrupt::Type::kCaptureCompare:
          nvic_ = NVICInterrupt(NVICInterrupt::kTIM8CaptureCompare);
          break;
        default:
          Assert(false, __FILE__, __LINE__, __func__);
          break;
      }
      interrupt_fn[7] = interrupt.fn;
      break;
    case TIM9:
    case TIM10:
    case TIM11:
    case TIM12:
    case TIM13:
    case TIM14:
    case TIM15:
    case TIM16:
    case TIM17:
      // Timer does not have an interrupt; Ignore the request.
      break;
    default:
      Assert(false, __FILE__, __LINE__, __func__, "Invalid Timer");
      break;
  }

  EnableInterrupt();
  nvic_.SetPriority(interrupt.priority);
}

void Timer::EnableInterrupt() const {
  nvic_.Enable();

  // TODO(Derppening): Expand ability to enable any type of interrupt.
  timer_enable_irq(timer_, TIM_DIER_UIE);
}

void Timer::DisableInterrupt() const {
  timer_disable_irq(timer_, TIM_DIER_UIE);
  nvic_.Disable();
}

void Timer::InitRcc() const {
  switch (timer_) {
    case TIM1:
      rcc_periph_clock_enable(RCC_TIM1);
      break;
    case TIM2:
      rcc_periph_clock_enable(RCC_TIM2);
      break;
    case TIM3:
      rcc_periph_clock_enable(RCC_TIM3);
      break;
    case TIM4:
      rcc_periph_clock_enable(RCC_TIM4);
      break;
    case TIM5:
      rcc_periph_clock_enable(RCC_TIM5);
      break;
    case TIM6:
      rcc_periph_clock_enable(RCC_TIM6);
      break;
    case TIM7:
      rcc_periph_clock_enable(RCC_TIM7);
      break;
    case TIM8:
      rcc_periph_clock_enable(RCC_TIM8);
      break;
    case TIM9:
      rcc_periph_clock_enable(RCC_TIM9);
      break;
    case TIM10:
      rcc_periph_clock_enable(RCC_TIM10);
      break;
    case TIM11:
      rcc_periph_clock_enable(RCC_TIM11);
      break;
    case TIM12:
      rcc_periph_clock_enable(RCC_TIM12);
      break;
    case TIM13:
      rcc_periph_clock_enable(RCC_TIM13);
      break;
    case TIM14:
      rcc_periph_clock_enable(RCC_TIM14);
      break;
    case TIM15:
      rcc_periph_clock_enable(RCC_TIM15);
      break;
    case TIM16:
      rcc_periph_clock_enable(RCC_TIM16);
      break;
    case TIM17:
      rcc_periph_clock_enable(RCC_TIM17);
      break;
    default:
      Assert(false, __FILE__, __LINE__, __func__, "Invalid Timer");
      break;
  }
}
}  // namespace rtlib::core::stm32f1

#endif  // defined(STM32F1)
