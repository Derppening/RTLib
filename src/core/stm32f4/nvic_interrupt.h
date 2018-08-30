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

#ifndef RTLIB_CORE_STM32F1_NVIC_INTERRUPT_H_
#define RTLIB_CORE_STM32F1_NVIC_INTERRUPT_H_

#if defined(STM32F4)

#include <libopencm3/cm3/nvic.h>

namespace rtlib::core::stm32f4 {

class NVICInterrupt final {
 public:
  /**
   * @brief Enumeration for different interrupt sources.
   *
   * This enum is intended to replace equvialent macros used in libopencm3. Descriptions of these enumerations are
   * taken from Section 10.1.2 (Interrupt and exception vectors) of ST RM0008.
   *
   * See http://libopencm3.org/docs/latest/stm32f1/html/group__CM3__nvic__defines__STM32F1.html.
   */
  enum Source : uint8_t {
    /**
     * @brief Window Watchdog interrupt.
     */
        kWWDG = NVIC_NVIC_WWDG_IRQ,
    /**
     * @brief PVD through EXTI Line detection interrupt.
     */
        kPVD = NVIC_PVD_IRQ,
    /**
     * @brief Tamper and TimeStamp interrupts through the EXTI line.
     */
        kTamperTimeStamp = NVIC_TAMP_STAMP_IRQ,
    /**
     * @brief RTC wakeup interrupt through the EXTI line.
     */
        kRTCWakeup = NVIC_RTC_WKUP_IRQ,
    /**
     * @brief Flash global interrupt.
     */
        kFlash = NVIC_FLASH_IRQ,
    /**
     * @brief RCC global interrupt.
     */
        kRCC = NVIC_RCC_IRQ,
    /**
     * @brief EXTI Line0 interrupt.
     */
        kEXTI0 = NVIC_EXTI0_IRQ,
    /**
     * @brief EXTI Line1 interrupt.
     */
        kEXTI1 = NVIC_EXTI1_IRQ,
    /**
     * @brief EXTI Line2 interrupt.
     */
        kEXTI2 = NVIC_EXTI2_IRQ,
    /**
     * @brief EXTI Line3 interrupt.
     */
        kEXTI3 = NVIC_EXTI3_IRQ,
    /**
     * @brief EXTI Line4 interrupt.
     */
        kEXTI4 = NVIC_EXTI4_IRQ,
    /**
     * @brief DMA1 Stream0 global interrupt.
     */
        kDMA1Str0 = NVIC_DMA1_STREAM0_IRQ,
    /**
     * @brief DMA1 Stream1 global interrupt.
     */
        kDMA1Str1 = NVIC_DMA1_STREAM1_IRQ,
    /**
     * @brief DMA1 Stream2 global interrupt.
     */
        kDMA1Str2 = NVIC_DMA1_STREAM2_IRQ,
    /**
     * @brief DMA1 Stream3 global interrupt.
     */
        kDMA1Str3 = NVIC_DMA1_STREAM3_IRQ,
    /**
     * @brief DMA1 Stream4 global interrupt.
     */
        kDMA1Str4 = NVIC_DMA1_STREAM4_IRQ,
    /**
     * @brief DMA1 Stream5 global interrupt.
     */
        kDMA1Str5 = NVIC_DMA1_STREAM5_IRQ,
    /**
     * @brief DMA1 Stream6 global interrupt.
     */
        kDMA1Str6 = NVIC_DMA1_STREAM6_IRQ,
    /**
     * @brief ADC1, ADC2 and ADC3 global interrupt.
     */
        kADC1To3 = NVIC_ADC_IRQ,
    /**
     * @brief CAN1 TX interrupts.
     */
        kCAN1Tx = NVIC_CAN1_TX_IRQ,
    /**
     * @brief CAN1 RX0 interrupts.
     */
        kCAN1Rx0 = NVIC_CAN1_RX0_IRQ,
    /**
     * @brief CAN1 RX1 interrupt.
     */
        kCAN1Rx1 = NVIC_CAN1_RX1_IRQ,
    /**
     * @brief CAN1 SCE interrupt.
     */
        kCAN1SCE = NVIC_CAN1_SCE_IRQ,
    /**
     * @brief EXTI Line[9:5] interrupts.
     */
        kEXTI9To5 = NVIC_EXTI9_5_IRQ,
    /**
     * @brief TIM1 Break interrupt and TIM9 global interrupt.
     */
        kTIM1Break = NVIC_TIM1_BRK_TIM9_IRQ,
    /**
     * @brief TIM1 Update interrupt and TIM10 global interrupt.
     */
        kTIM1Update = NVIC_TIM1_UP_TIM10_IRQ,
    /**
     * @brief TIM1 Trigger and Commutation interrupts and TIM11 global interrupt.
     */
        kTIM1TriggerAndCommutation = NVIC_TIM1_TRG_COM_TIM11_IRQ,
    /**
     * @brief TIM1 Capture Compare interrupt.
     */
        kTIM1CaptureCompare = NVIC_TIM1_CC_IRQ,
    /**
     * @brief TIM2 global interrupt.
     */
        kTIM2 = NVIC_TIM2_IRQ,
    /**
     * @brief TIM3 global interrupt.
     */
        kTIM3 = NVIC_TIM3_IRQ,
    /**
     * @brief TIM4 global interrupt.
     */
        kTIM4 = NVIC_TIM4_IRQ,
    /**
     * @brief I2C1 event interrupt.
     */
        kI2C1Event = NVIC_I2C1_EV_IRQ,
    /**
     * @brief I2C1 error interrupt.
     */
        kI2C1Error = NVIC_I2C1_ER_IRQ,
    /**
     * @brief I2C2 event interrupt.
     */
        kI2C2Event = NVIC_I2C2_EV_IRQ,
    /**
     * @brief I2C2 error interrupt.
     */
        kI2C2Error = NVIC_I2C2_ER_IRQ,
    /**
     * @brief SPI1 global interrupt.
     */
        kSPI1 = NVIC_SPI1_IRQ,
    /**
     * @brief SPI2 global interrupt.
     */
        kSPI2 = NVIC_SPI2_IRQ,
    /**
     * @brief USART1 global interrupt.
     */
        kUSART1 = NVIC_USART1_IRQ,
    /**
     * @brief USART2 global interrupt.
     */
        kUSART2 = NVIC_USART2_IRQ,
    /**
     * @brief USART3 global interrupt.
     */
        kUSART3 = NVIC_USART3_IRQ,
    /**
     * @brief EXTI Line[15:10] interrupts.
     */
        kEXTI15To10 = NVIC_EXTI15_10_IRQ,
    /**
     * @brief RTC alarm through EXTI line interrupt.
     */
        kRTCAlarm = NVIC_RTC_ALARM_IRQ,
    /**
     * @brief USB On-The-Go FS Wakeup through EXTI line interrupt.
     */
        kOTGFSWakeup = NVIC_OTG_FS_IRQ,
    /**
     * @brief TIM8 Break interrupt and TIM12 global interrupt.
     */
        kTIM8Break = NVIC_TIM8_BRK_TIM12_IRQ,
    /**
     * @brief TIM8 Update interrupt and TIM13 global interrupt.
     */
        kTIM8Update = NVIC_TIM8_UP_TIM13_IRQ,
    /**
     * @brief TIM8 Trigger and Commutation interrupts and TIM14 global interrupt.
     */
        kTIM8TriggerAndCommutation = NVIC_TIM8_TRG_COM_TIM14_IRQ,
    /**
     * @brief TIM8 Capture Compare interrupt.
     */
        kTIM8CaptureCompare = NVIC_TIM8_CC_IRQ,
    /**
     * @brief DMA1 Stream7 global interrupt.
     */
        kDMA1Str7 = NVIC_DMA1_STREAM7_IRQ,
    /**
     * @brief FSMC global interrupt.
     */
        kFSMC = NVIC_FSMC_IRQ,
    /**
     * @brief SDIO global interrupt.
     */
        kSDIO = NVIC_SDIO_IRQ,
    /**
     * @brief TIM5 global interrupt.
     */
        kTIM5 = NVIC_TIM5_IRQ,
    /**
     * @brief SPI3 global interrupt.
     */
        kSPI3 = NVIC_SPI3_IRQ,
    /**
     * @brief UART4 global interrupt.
     */
        kUART4 = NVIC_UART4_IRQ,
    /**
     * @brief UART5 global interrupt.
     */
        kUART5 = NVIC_UART5_IRQ,
    /**
     * @brief TIM6 global interrupt.
     */
        kTIM6 = NVIC_TIM6_DAC_IRQ,
    /**
     * @brief DAC1 and DAC2 underrun error interrupts.
     */
        kDAC1AndDAC2Underrun = NVIC_TIM6_DAC_IRQ,
    /**
     * @brief TIM7 global interrupt.
     */
        kTIM7 = NVIC_TIM7_IRQ,
    /**
     * @brief DMA2 Stream0 global interrupt.
     */
        kDMA2Str0 = NVIC_DMA2_STREAM0_IRQ,
    /**
     * @brief DMA2 Stream1 global interrupt.
     */
        kDMA2Str1 = NVIC_DMA2_STREAM1_IRQ,
    /**
     * @brief DMA2 Stream2 global interrupt.
     */
        kDMA2Str2 = NVIC_DMA2_STREAM2_IRQ,
    /**
     * @brief DMA2 Stream3 global interrupt.
     */
        kDMA2Str3 = NVIC_DMA2_STREAM3_IRQ,
    /**
     * @brief DMA2 Stream4 global interrupt.
     */
        kDMA2Str4 = NVIC_DMA2_STREAM4_IRQ,
    /**
     * @brief Ethernet global interrupt.
     */
        kEthernet = NVIC_ETH_IRQ,
    /**
     * @brief Ethernet Wakeup through EXTI line interrupt.
     */
        kEthernetWakeup = NVIC_ETH_WKUP_IRQ,
    /**
     * @brief CAN2 TX interrupts.
     */
        kCAN2Tx = NVIC_CAN2_TX_IRQ,
    /**
     * @brief CAN2 RX0 interrupts.
     */
        kCAN2Rx0 = NVIC_CAN2_RX0_IRQ,
    /**
     * @brief CAN2 RX1 interrupt.
     */
        kCAN2Rx1 = NVIC_CAN2_RX1_IRQ,
    /**
     * @brief CAN2 SCE interrupt.
     */
        kCAN2SCE = NVIC_CAN2_SCE_IRQ,
    /**
     * @brief USB On-The-Go FS global interrupt.
     */
        kOTGFS = NVIC_OTG_FS_IRQ,
    /**
     * @brief DMA2 Stream5 global interrupt.
     */
        kDMA2Str5 = NVIC_DMA2_STREAM5_IRQ,
    /**
     * @brief DMA2 Stream6 global interrupt.
     */
        kDMA2Str6 = NVIC_DMA2_STREAM6_IRQ,
    /**
     * @brief DMA Stream7 global interrupt.
     */
        kDMA2Str7 = NVIC_DMA2_STREAM7_IRQ,
    /**
     * @brief USART6 global interrupt.
     */
        kUSART6 = NVIC_USART6_IRQ,
    /**
     * @brief I2C3 event interrupt.
     */
        kI2C3Event = NVIC_I2C3_EV_IRQ,
    /**
     * @brief I2C3 error interrupt.
     */
        kI2C3Error = NVIC_I2C3_ER_IRQ,
    /**
     * @brief USB On-The-Go High Speed Endpoint 1 Out global interrupt.
     */
        kOTGHSEp1Out = NVIC_OTG_HS_EP1_OUT_IRQ,
    /**
     * @brief USB On-The-Go High Speed Endpoint 1 In global interrupt.
     */
        kOTGHSEp1In = NVIC_OTG_HS_EP1_IN_IRQ,
    /**
     * @brief USB On-The-Go High Speed Wakeup through EXTI interrupt.
     */
        kOTGHSWakeup = NVIC_OTG_HS_WKUP_IRQ,
    /**
     * @brief USB On-The-Go High Speed global interrupt.
     */
        kOTGHS = NVIC_OTG_HS_IRQ,
    /**
     * @brief DCMI global interrupt.
     */
        kDCMI = NVIC_DCMI_IRQ,
    /**
     * @brief CRYP crypto global interrupt.
     */
        kCrypto = NVIC_CRYP_IRQ,
    /**
     * @brief Hash global interrupt.
     *
     * @see #kRNG
     */
        kHash = NVIC_HASH_RNG_IRQ,
    /**
     * @brief Rng global interrupt.
     *
     * @see #kHash
     */
        kRNG = NVIC_HASH_RNG_IRQ,
    /**
     * @brief FPU global interrupt.
     */
        kFPU = NVIC_FPU_IRQ
  };

  /**
   * @brief Default constructor.
   *
   * Initializes this object to an "unbinded" state, i.e. this object does not manage any interrupts.
   */
  constexpr NVICInterrupt() : nvic_(kUnbindedState) {}
  /**
   * @brief Conversion constructor.
   *
   * @param[in] source Interrupt source to manage. See NVICInterrupt#Source.
   */
  constexpr explicit NVICInterrupt(Source source) : nvic_(uint8_t(source)) {}

  /**
   * @brief Destructor.
   *
   * Side effects: Clears all pending interrupts and disables the interrupt managed by this object.
   */
  ~NVICInterrupt();

  /**
   * @brief Move constructor.
   *
   * @param[in] other NVICInterrupt object to move from.
   */
  NVICInterrupt(NVICInterrupt&& other) noexcept :
      nvic_(other.nvic_) {
    other.nvic_ = kUnbindedState;
  }
  /**
   * @brief Move assignment operator.
   *
   * @param[in] other NVICInterrupt object to move from.
   * @return Reference to the moved NVICInterrupt.
   */
  constexpr NVICInterrupt& operator=(NVICInterrupt&& other) noexcept {
    if (this != &other) {
      nvic_ = other.nvic_;
      other.nvic_ = kUnbindedState;
    }

    return *this;
  }

  /**
   * @brief Copy constructor.
   *
   * This constructor is deleted because there should only be one object managing each NVICInterrupt, similar to @c
   * std::unique_ptr.
   */
  NVICInterrupt(const NVICInterrupt&) = delete;
  /**
   * @brief Copy assignment operator.
   *
   * This constructor is deleted because there should only be one object managing each NVICInterrupt, similar to @c
   * std::unique_ptr.
   */
  NVICInterrupt& operator=(const NVICInterrupt&) = delete;

  /**
   * @return Whether this object is managing an interrupt.
   */
  constexpr bool IsBinded() const { return nvic_ != kUnbindedState; }

  /**
   * @brief Enables this interrupt.
   *
   * If validation is enabled, this function will assert if invoked on a non-owning object.
   */
  void Enable() const;
  /**
   * @brief Disables this interrupt.
   *
   * If validation is enabled, this function will assert if invoked on a non-owning object.
   */
  void Disable() const;
  /**
   * @brief Sets the priority of this interrupt.
   *
   * If validation is enabled, this function will assert if invoked on a non-owning object.
   *
   * @param[in] priority The new priority of this interrupt.
   */
  void SetPriority(uint8_t priority) const;

  /**
   * @return Whether this interrupt is pending service.
   * Will always return @c false if this object is not managing an interrupt.
   */
  bool HasPending() const;

 private:
  /**
   * @brief Constant for representing an object not managing an interrupt.
   */
  static constexpr const uint8_t kUnbindedState = uint8_t(-1);

  uint8_t nvic_;
};

}  // namespace rtlib::core::stm32f4

#endif  // defined(STM32F4)

#endif  // RTLIB_CORE_STM32F1_NVIC_INTERRUPT_H_
