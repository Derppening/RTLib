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

#ifndef RTLIB_CORE_STM32F4_TIMER_H_
#define RTLIB_CORE_STM32F4_TIMER_H_

#if defined(STM32F4)

#include <libopencm3/stm32/timer.h>

#include "core/stm32f4/nvic_interrupt.h"

namespace rtlib::core::stm32f4 {

class Timer {
 public:
  /**
   * @brief Enumeration for different internal timers.
   */
  enum Source : uint32_t {
    /**
     * @brief Advanced-control timer TIM1.
     */
        kTIM1 = TIM1,
    /**
     * @brief General-purpose timer TIM2.
     */
        kTIM2 = TIM2,
    /**
     * @brief General-purpose timer TIM3.
     */
        kTIM3 = TIM3,
    /**
     * @brief General-purpose timer TIM4.
     */
        kTIM4 = TIM4,
    /**
     * @brief General-purpose timer TIM5.
     */
        kTIM5 = TIM5,
    /**
     * @brief Basic timer TIM6.
     */
        kTIM6 = TIM6,
    /**
     * @brief Basic timer TIM7.
     */
        kTIM7 = TIM7,
    /**
     * @brief Advanced-control timer TIM8.
     */
        kTIM8 = TIM8,
    /**
     * @brief General-purpose timer TIM9.
     */
        kTIM9 = TIM9,
    /**
     * @brief General-purpose timer TIM10.
     */
        kTIM10 = TIM10,
    /**
     * @brief General-purpose timer TIM11.
     */
        kTIM11 = TIM11,
    /**
     * @brief General-purpose timer TIM12.
     */
        kTIM12 = TIM12,
    /**
     * @brief General-purpose timer TIM13.
     */
        kTIM13 = TIM13,
    /**
     * @brief General-purpose timer TIM14.
     */
        kTIM14 = TIM14
  };

  /**
   * @brief Enumeration for different clock divisions.
   */
  enum struct ClockDivisionRatio : uint32_t {
    /**
     * @brief Set the dead-time and sampling clock to match the timer clock's frequency.
     */
        kUnchanged = TIM_CR1_CKD_CK_INT,
    /**
     * @brief Set the dead-time and sampling clock to half of the timer clock's frequency.
     */
        kDivTwo = TIM_CR1_CKD_CK_INT_MUL_2,
    /**
     * @brief Set the dead-time and sampling clock to quarter of the timer clock's frequency.
     */
        kDivFour = TIM_CR1_CKD_CK_INT_MUL_4
  };

  /**
   * @brief Enumeration for different mode-alignment selection.
   */
  enum struct CenterAlignedMode : uint32_t {
    /**
     * @brief Edge-aligned mode. The counter will count up or down depending on Timer#Direction.
     */
        kEdgeAligned = TIM_CR1_CMS_EDGE,
    /**
     * @brief Center-aligned mode 1. The counter counts up and down alternatively. Output compare interrupt flags are
     * set when the counter is counting down.
     */
        kCenterAligned1 = TIM_CR1_CMS_CENTER_1,
    /**
     * @brief Center-aligned mode 2. The counter counts up and down alternatively. Output compare interrupt flags are
     * set when the counter is counting up.
     */
        kCenterAligned2 = TIM_CR1_CMS_CENTER_2,
    /**
     * @brief Center-aligned mode 3. The counter counts up and down alternatively. Output compare interrupt flags are
     * set when the counter is counting (regardless of direction).
     */
        kCenterAligned3 = TIM_CR1_CMS_CENTER_3
  };

  /**
   * @brief Enumeration for different counter directions.
   */
  enum struct Direction : uint32_t {
    /**
     * @brief Counter used as up-counter.
     */
        kUp = TIM_CR1_DIR_UP,
    /**
     * @brief Counter used as down-counter.
     */
        kDown = TIM_CR1_DIR_DOWN
  };

  /**
   * @brief Type definition for timer interrupt handler function.
   */
  using HandlerFn = void (*)();

  /**
   * @brief Configuration for timer interrupts.
   */
  struct Interrupt {
    /**
     * @brief Type of interrupt to enable.
     */
    enum struct Type {
      kBreak,
      kUpdate,
      kTriggerAndCommutation,
      kCaptureCompare
    };

    /**
     * @brief Timer Interrupt Handler function.
     *
     * When invoked as an argument to SetEnableInterrupt, all interrupts from that Timer will be redirected to this
     * function. This function must have the prototype equivalent to HandlerFn.
     */
    HandlerFn fn = nullptr;
    /**
     * @brief Priority for the interrupt.
     */
    uint8_t priority;
    /**
     * @brief Type of interrupt to enable.
     *
     * This field is only used for TIM1 and TIM8 advanced-control timers.
     */
    Type type;
  };

  /**
   * @brief Configuration for Timer.
   */
  struct Config {
    /**
     * @brief Timer to manage.
     *
     * Defaults to an indeterminate state.
     */
    Source timer;
    /**
     * @brief Clock division ratio between DTS clock and timer clock.
     *
     * Defaults to ClockDivisionRatio#kUnchanged.
     */
    ClockDivisionRatio clock_div = ClockDivisionRatio::kUnchanged;
    /**
     * @brief Alignment mode.
     *
     * Defaults to CenterAlignedMode#kEdgeAligned.
     */
    CenterAlignedMode alignment = CenterAlignedMode::kEdgeAligned;
    /**
     * @brief Direction to count.
     *
     * Defaults to Direction#kUp.
     */
    Direction direction = Direction::kUp;
    /**
     * @brief Prescaler value.
     *
     * Defaults to 0 (prescaler = 1).
     */
    uint16_t prescaler;
    /**
     * @brief Period of each cycle per tick. This field should be non-zero under normal use.
     *
     * Defaults to 0.
     */
    uint16_t period;
    /**
     * @brief Number to start counting from.
     *
     * Defaults to 0.
     */
    uint16_t start_count = 0;
  };

  /**
   * @brief Conversion constructor.
   *
   * @param[in] config Configuration for the Timer. See Timer#Config.
   */
  explicit Timer(const Config& config);

  Timer(Source timer,
        ClockDivisionRatio clock_div,
        CenterAlignedMode alignment,
        Direction direction,
        uint32_t prescaler,
        uint32_t period,
        uint32_t start_count);

  /**
   * @brief Destructor.
   *
   * Side effects: Disables and resets the managing timer to its default state.
   */
  virtual ~Timer();

  /**
   * @param[in] count New counter value.
   */
  void SetCounterValue(uint32_t count) const;
  /**
   * @param[in] period New clock period in ticks.
   */
  void SetPeriod(uint32_t period) const;
  /**
   * @note The actual prescaler is one bigger than the prescaler specified here.
   *
   * @param[in] prescaler New prescaler value.
   */
  void SetPrescaler(uint32_t prescaler) const;
  /**
   * @param[in] value New repetition counter value.
   */
  void SetReptitionCounter(uint32_t value) const;

  /**
   * @brief Sets whether to enable auto-reload preload.
   *
   * @param state If true, buffer the auto-reload register.
   */
  void SetPreload(bool state) const;
  /**
   * @brief Sets whether to stop at the next update event.
   *
   * @param state If true, stop at the next update event; Otherwise, restart counting from a pre-determined point.
   */
  void StopAtUpdateEvent(bool state) const;
  /**
   * @brief Sets whether to activate this timer.
   *
   * @param state New state of the timer.
   */
  void SetState(bool state) const;

  void Start() const { SetState(true); }
  void Stop() const { SetState(false); }

  /**
   * @brief Sets and enables the interrupt associated with this timer.
   *
   * @param interrupt Interrupt configuration. See Timer#Interrupt.
   */
  void SetEnableInterrupt(const Interrupt& interrupt);
  /**
   * @brief Enables the interrupt associated with this timer.
   */
  void EnableInterrupt() const;
  /**
   * @brief Disables the interrupt associated with this timer.
   */
  void DisableInterrupt() const;

  uint32_t timer_;

 private:
  void InitRcc() const;

  NVICInterrupt nvic_;
};

}  // namespace rtlib::core::stm32f4

#endif  // defined(STM32F4)

#endif  // RTLIB_CORE_STM32F4_TIMER_H_
