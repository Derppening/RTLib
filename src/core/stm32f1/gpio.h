#ifndef RTLIB_CORE_STM32F1_GPIO_H_
#define RTLIB_CORE_STM32F1_GPIO_H_

#if defined(STM32F1)

#include <cstdint>
#include <utility>

#include <libopencm3/stm32/gpio.h>

#include "core/util.h"

namespace core {
namespace stm32f1 {

/**
 * @brief STM32F1xx-specific HAL implementation for GPIO pins.
 *
 * This abstraction layer provides simple Read/Write operations on GPIO pins. One GPIO object will correspond and manage
 * one GPIO pinout from the MCU.
 */
class GPIO final {
 public:
  /**
   * @brief Type definition for alternate function selection.
   */
  using AltFnMaps = uint32_t;

  /**
   * @brief Enumeration for different GPIO configurations.
   *
   * This enum is intended to replace equivalent macros used in libopencm3.
   *
   * See http://libopencm3.org/docs/latest/stm32f1/html/group__gpio__cnf.html.
   */
  enum struct Configuration : uint8_t {
    /**
     * @brief Use this GPIO as analog input.
     *
     * Equivalent to libopencm3 macro @c GPIO_CNF_INPUT_ANALOG.
     */
    kInputAnalog = 0x0,
    /**
     * @brief Use this GPIO as floating input.
     *
     * Equivalent to libopencm3 macro @c GPIO_CNF_INPUT_FLOAT.
     */
    kInputFloat,
    /**
     * @brief Use this GPIO as pull-up/pull-down input.
     *
     * Equivalent to libopencm3 macro @c GPIO_CNF_INPUT_PULL_UPDOWN.
     */
    kInputPullUpDown,
    /**
     * @brief Use this GPIO as digital push-pull output.
     *
     * Equivalent to libopencm3 macro @c GPIO_CNF_OUTPUT_PUSHPULL
     */
    kOutputPushPull = 0x0,
    /**
     * @brief Use this GPIO as digital open-drain output.
     *
     * Equivalent to libopencm3 macro @c GPIO_CNF_OUTPUT_OPENDRAIN.
     */
    kOutputOpenDrain,
    /**
     * @brief Use this GPIO as alternate function push-pull output.
     *
     * Equivalent to libopencm3 macro @c GPIO_CNF_ALTFN_PUSHPULL.
     */
    kOutputAltFnPushPull,
    /**
     * @brief Use this GPIO as alternate function open-drain output.
     *
     * Equivalent to libopencm3 macro @c GPIO_CNF_ALTFN_OPENDRAIN.
     */
    kOutputAltFnOpenDrain
  };

  /**
   * @brief Enumeration for different GPIO modes.
   *
   * This enum is intended to replace equivalent macros used in libopencm3.
   *
   * See http://libopencm3.org/docs/latest/stm32f1/html/group__gpio__mode.html.
   */
  enum struct Mode : uint8_t {
    /**
     * @brief Input Mode.
     *
     * Equivalent to libopencm3 macro @c GPIO_MODE_INPUT.
     */
    kInput,
    /**
     * @brief Output Mode at 10MHz max.
     *
     * Equivalent to libopencm3 macro @c GPIO_MODE_OUTPUT_10MHZ.
     */
    kOutput10MHz,
    /**
     * @brief Output Mode at 2MHz max.
     *
     * Equivalent to libopencm3 macro @c GPIO_MODE_OUTPUT_2MHZ.
     */
    kOutput2MHz,
    /**
     * @brief Output Mode at 50MHz max.
     *
     * Equivalent to libopencm3 macro @c GPIO_MODE_OUTPUT_50MHZ.
     */
    kOutput50MHz
  };

  /**
   * @brief Enumeration for different Serial Wire JTAG states.
   *
   * This enum is intended to replace equivalent macros used in libopencm3.
   *
   * See http://libopencm3.org/docs/latest/stm32f1/html/group__afio__swj__disable.html.
   */
  enum struct JTAGDisables : uint32_t {
    /**
     * @brief Use full JTAG capability.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_SWJ_CFG_FULL_SWJ.
     */
    kNoDisable = AFIO_MAPR_SWJ_CFG_FULL_SWJ,
    /**
     * @brief Use full JTAG capability with JNTRST.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_SWJ_CFG_FULL_SWJ_NO_JNTRST.
     */
    kNoJNTRST = AFIO_MAPR_SWJ_CFG_FULL_SWJ_NO_JNTRST,
    /**
     * @brief Disable JTAG but enable SWD.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON.
     */
    kDisableJTAG = AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON,
    /**
     * @brief Disable both JTAG and SWD.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_OFF.
     */
    kDisableJTAGSW = AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_OFF
  };

  /**
   * @brief Enumeration for the primary set of different alternate function remaps.
   *
   * This enum is intended to provide a type-safe container for the full list of remaps.
   *
   * See the libopencm3 documentation for the full list of remaps:
   * - Alternate Function Remaps: http://libopencm3.org/docs/latest/stm32f1/html/group__afio__remap.html
   * - CAN 1 Alternate Function Remaps: http://libopencm3.org/docs/latest/stm32f1/html/group__afio__remap__can1.html
   * - Timer 3 Alternate Function Remaps: http://libopencm3.org/docs/latest/stm32f1/html/group__afio__remap__tim3.html
   * - Timer 2 Alternate Function Remaps: http://libopencm3.org/docs/latest/stm32f1/html/group__afio__remap__tim2.html
   * - Timer 1 Alternate Function Remaps: http://libopencm3.org/docs/latest/stm32f1/html/group__afio__remap__tim1.html
   * - USART 3 Alternate Function Remaps: http://libopencm3.org/docs/latest/stm32f1/html/group__afio__remap__usart3.html
   * - Connectivity Alternate Function Remaps:
   * http://libopencm3.org/docs/latest/stm32f1/html/group__afio__remap__cld.html
   */
  enum struct PriRemap : uint32_t {
    /**
     * @brief ADC2 External Trigger Regulator Conversion Remapping.
     *
     * By default, this is connected to EXTI11. Enabling this remap will connect ADC2 External Event Regular Conversion
     * to TIM8_TRGO.
     *
     * @note This remap is only available for STM32F101/102/103 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_ADC2_ETRGREG_REMAP.
     */
    kADC2ExtTrigReg = AFIO_MAPR_ADC2_ETRGREG_REMAP,
    /**
     * @brief ADC2 External Trigger Injected Conversion Remapping.
     *
     * By default, this is connected to EXTI15. Enabling this remap will connect ADC2 External Event Injected Conversion
     * to TIM8_Channel4.
     *
     * @note This remap is only available for STM32F101/102/103 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_ADC2_ETRGINJ_REMAP.
     */
    kADC2ExtTrigInj = AFIO_MAPR_ADC2_ETRGINJ_REMAP,
    /**
     * @brief ADC1 External Trigger Regulator Conversion Remapping.
     *
     * By default, this is connected to EXTI11. Enabling this remap will connect ADC1 External Event Regular Conversion
     * to TIM8_TRGO.
     *
     * @note This remap is only available for STM32F101/102/103 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_ADC1_ETRGREG_REMAP.
     */
    kADC1ExtTrigReg = AFIO_MAPR_ADC1_ETRGREG_REMAP,
    /**
     * @brief ADC1 External Trigger Injected Conversion Remapping.
     *
     * By default, this is connected to EXTI15. Enabling this remap will connect ADC2 External Event Injected Conversion
     * to TIM8_Channel4.
     *
     * @note This remap is only available for STM32F101/102/103 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_ADC1_ETRGINJ_REMAP.
     */
    kADC1ExtTrigInj = AFIO_MAPR_ADC1_ETRGINJ_REMAP,
    /**
     * @brief TIM5 Channel 4 Internal Remapping.
     *
     * By default, TIM5_CH4 is connected to PA3. Enabling this remap will connect the LSI internal clock to TIM5_CH4 for
     * calibration.
     *
     * @note This remap is only available for High Density STM32F100 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_TIM5CH4_IREMAP.
     */
    kTIM5Ch4 = AFIO_MAPR_TIM5CH4_IREMAP,
    /**
     * @brief Port D0/D1 Remapping.
     *
     * By default, PD0 and PD1 are allocated for GPIO. Enabling this remap will connect PD0 and PD1 to OSC_IN and
     * OSC_OUT respectively.
     *
     * @note This remap is only useful for 36-pin, 48-pin and 64-pin packages.
     *
     * Equivalent to libopecm3 macro @c AFIO_MAPR_PD01_REMAP.
     */
    kPD01 = AFIO_MAPR_PD01_REMAP,
    /**
     * @brief TIM4 CH1/CH2/CH3/CH4 Remapping.
     *
     * By default, TIM4 channels are connected to PB6-PB9. Enabling this remap will connect the channels to PD12-PD15.
     *
     * @note TIM4_ETR (PE0) will not be remapped.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_TIM4_REMAP.
     */
    kTIM4 = AFIO_MAPR_TIM4_REMAP,
    /**
     * @brief USART2 CTS/RTS/TX/RX/CK Remapping.
     *
     * By default, USART2 pins are connected to PA0-PA4. Enabling this remap will connect the pins to PD3-PD7.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_USART2_REMAP.
     */
    kUSART2 = AFIO_MAPR_USART2_REMAP,
    /**
     * @brief USART1 TX/RX Remapping.
     *
     * By default, USART1 pins are connected to PA9-PA10. Enabling this remap will connect the pins to PB6-PB7.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_USART1_REMAP.
     */
    kUSART1 = AFIO_MAPR_USART1_REMAP,
    /**
     * @brief I2C1 SCL/SDA Remapping.
     *
     * By default, I2C1 pins are connected to PB6-PB7. Enabling this remap will connect the pins to PB8-PB9.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_I2C1_REMAP.
     */
    kI2C1 = AFIO_MAPR_I2C1_REMAP,
    /**
     * @brief SPI1 NSS/SCK/MISO/MOSI Remapping.
     *
     * By default, SPI1 pins are connected to PA4-PA7. Enabling this remap will connect the pins to PA15/PB3-5.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_SPI1_REMAP.
     */
    kSPI1 = AFIO_MAPR_SPI1_REMAP,
    /**
     * @brief CAN1 RX/TX Port A Remapping.
     *
     * Enabling this remap will connect CAN1 pins to PA11-PA12.
     *
     * @note This remap is only available for STM32F101/102/103/105/107 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_CAN1_REMAP_PORTA.
     */
    kCAN1PA = AFIO_MAPR_CAN1_REMAP_PORTA,
    /**
     * @brief CAN1 RX/TX Port B Remapping.
     *
     * Enabling this remap will connect CAN1 pins to PB8-PB9.
     *
     * @note This remap is only available for STM32F101/102/103/105/107 devices.
     * @note This remap is not available on 36-pin packages.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_CAN1_REMAP_PORTB.
     */
    kCAN1PB = AFIO_MAPR_CAN1_REMAP_PORTB,
    /**
     * @brief CAN1 RX/TX Port D Remapping.
     *
     * Enabling this remap will connect CAN1 pins to PD0-PD1.
     *
     * @note This remap is only available for STM32F101/102/103/105/107 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_CAN1_REMAP_PORTD.
     */
    kCAN1PD = AFIO_MAPR_CAN1_REMAP_PORTD,
    /**
     * @brief TIM3 No Remapping. (Default)
     *
     * Enabling this will disable remaps and connect the channels to PA6-PA7/PB0-PB1.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_TIM3_REMAP_NO_REMAP.
     */
    kTIM3NoRemap = AFIO_MAPR_TIM3_REMAP_NO_REMAP,
    /**
     * @brief TIM3 CH1-CH2 Partial Remapping.
     *
     * Enabling this will connect TIM3 CH1-CH2 to PB4-PB5.
     *
     * @note TIM3_ETR (PE0) will not be remapped.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_TIM3_REMAP_PARTIAL_REMAP.
     */
    kTIM3PartRemap = AFIO_MAPR_TIM3_REMAP_PARTIAL_REMAP,
    /**
     * @brief TIM3 CH1-CH4 Full Remapping.
     *
     * Enabling this will connect TIM3 channels to PC6-PC9.
     *
     * @note TIM3_ETR (PE0) will not be remapped.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_TIM3_REMAP_FULL_REMAP.
     */
    kTIM3FullRemap = AFIO_MAPR_TIM3_REMAP_FULL_REMAP,
    /**
     * @brief TIM2 No Remapping. (Default)
     *
     * Enabling this will disable remaps and connect the channels to PA0-PA3.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_TIM2_REMAP_NO_REMAP.
     */
    kTIM2NoRemap = AFIO_MAPR_TIM2_REMAP_NO_REMAP,
    /**
     * @brief TIM2 ETR/CH1-CH2 Partial Remapping (1).
     *
     * Enabling this will connect TIM2 CH1-CH2 to PA15/PB3.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_TIM2_REMAP_PARTIAL_REMAP1.
     */
    kTIM2PartRemap1 = AFIO_MAPR_TIM2_REMAP_PARTIAL_REMAP1,
    /**
     * @brief TIM2 ETR/CH1-CH4 Partial Remapping (2).
     *
     * Enabling this will connect TIM2 CH3-CH4 to PB10-PB11.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_TIM2_REMAP_PARTIAL_REMAP2.
     */
    kTIM2PartRemap2 = AFIO_MAPR_TIM2_REMAP_PARTIAL_REMAP2,
    /**
     * @brief TIM2 ETR/CH1-CH4 Full Remapping.
     *
     * Enabling this will connect TIM2 channels to PA15/PB3/PB10-11.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_TIM2_REMAP_PARTIAL_REMAP1.
     */
    kTIM2FullRemap = AFIO_MAPR_TIM2_REMAP_FULL_REMAP,
    /**
     * @brief TIM1 No Remapping. (Default)
     *
     * Enabling this will disable remaps and connect the channels to PA12/PA8-PA11/PB12-PB15.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_TIM1_REMAP_NO_REMAP.
     */
    kTIM1NoRemap = AFIO_MAPR_TIM1_REMAP_NO_REMAP,
    /**
     * @brief TIM1 BKIN/CH1N-CH3N Partial Remapping.
     *
     * Enabling this will connect TIM1 BKIN/CH1N-CH3N to PA6-PA7/PB0-PB1.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_TIM1_REMAP_PARTIAL_REMAP.
     */
    kTIM1PartRemap = AFIO_MAPR_TIM1_REMAP_PARTIAL_REMAP,
    /**
     * @brief TIM1 ETR/CH1-CH4/BKIN/CH1N-CH3N Full Remapping.
     *
     * Enabling this will connect TIM1 channels to PE7/PE9/PE11/PE13-15/PE8/PE10/PE12.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_TIM1_REMAP_FULL_REMAP.
     */
    kTIM1FullRemap = AFIO_MAPR_TIM1_REMAP_FULL_REMAP,
    /**
     * @brief USART3 No Remapping. (Default)
     *
     * Enabling this will disable remaps and connect USART3 channels to PB10-PB14.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_USART3_REMAP_NO_REMAP.
     */
    kUSART3NoRemap = AFIO_MAPR_USART3_REMAP_NO_REMAP,
    /**
     * @brief USART3 TX/RX/CK Partial Remapping.
     *
     * Enabling this will connect USART3 TX/RX/CK to PC10-PC12.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_USART3_REMAP_PARTIAL_REMAP.
     */
    kUSART3PartRemap = AFIO_MAPR_USART3_REMAP_PARTIAL_REMAP,
    /**
     * @brief USART3 TX/RX/CK/CTS/RTS Full Remapping.
     *
     * Enabling this will connect USART3 pins to PD8-PD12.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_USART3_REMAP_FULL_REMAP.
     */
    kUSART3FullRemap = AFIO_MAPR_USART3_REMAP_FULL_REMAP,
    /**
     * @brief Ethernet PTP PPS Remapping.
     *
     * By default, Ethernet MAC PTP_PPS will not be output. Enabling this remap will connect PTP_PPS to PB5.
     *
     * @note This remap is only available for STM32F105/107 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_PTP_PPS_REMAP.
     */
    kPTPPPS = AFIO_MAPR_PTP_PPS_REMAP,
    /**
     * @brief TIM2 Internal Trigger 1 Remapping.
     *
     * By default, TIM2_ITR1 is connected internally to the Ethernet PTP output. Enabling this remap will connect USB
     * OTG Start-of-Frame output to TIM2_ITR1.
     *
     * @note This remap is only available for STM32F105/107 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_TIM2ITR1_IREMAP.
     */
    kTIM2IntTrig1 = AFIO_MAPR_TIM2ITR1_IREMAP,
    /**
     * @brief SPI3 NSS/SCK/MISO/MOSI and I2S3 WS/CK/SD Remapping.
     *
     * By default, SPI3/I2S3 are connected to PA15/PB3-PB5. Enabling this remap will connect the pins to PA4/PC10-PC12.
     *
     * @note This remap is only available for STM32F105/107 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_SPI3_REMAP.
     */
    kSPI3 = AFIO_MAPR_SPI3_REMAP,
    /**
     * @brief MII/RMII Selection.
     *
     * By default, the Ethernet MAC is configured for connecting with a MII PHY. Enabling this remap will configure the
     * the MAC for connecting with a RMII PHY.
     *
     * @note This remap is only available for STM32F105/107 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_MII_RMII_SEL.
     */
    kMIIOrRMIISelect = AFIO_MAPR_MII_RMII_SEL,
    /**
     * @brief CAN2 RX/TX Remapping.
     *
     * By default, CAN2 pins are connected to PB12-PB13. Enabling this remap will connect the pins to PB5-PB6.
     *
     * @note This remap is only available for STM32F105/107 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_CAN2_REMAP.
     */
    kCAN2 = AFIO_MAPR_CAN2_REMAP,
    /**
     * @brief Ethernet MAC DV/RXD0-RXD3 Remapping.
     *
     * By default, Ethernet MAC pins are connected to PA7/PC4-PC5/PB0-PB1. Enabling this remap will connect the pins to
     * PD8-PD12.
     *
     * @note This remap is only available for STM32F105/107 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR_ETH_REMAP.
     */
    kEthMAC = AFIO_MAPR_ETH_REMAP
  };

  /**
   * @brief Enumeration for the secondary set of different alternate function remaps.
   *
   * This enum is intended to provide a type-safe container for the full list of remaps.
   *
   * See http://libopencm3.org/docs/latest/stm32f1/html/group__afio__remap2.html.
   */
  enum struct SecRemap : uint32_t {
    /**
     * @brief Miscellaneous Features Remapping.
     *
     * By default, DMA2 CH5 is mapped in the vector table at position 59 (with DMA CH4), TIM5 TRGO event is selected as
     * DAC Trigger 3, and TIM5 triggers TIM1/3. Enabling this remap will map DMA2 CH5 to position 60 of the vector
     * table, TIM15 TRGO event is selected as DAC Trigger 3, and TIM15 triggers TIM1/3.
     *
     * @note This remap is only available for High Density STM32F100 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR2_MISC_REMAP.
     */
    kMisc = AFIO_MAPR2_MISC_REMAP,
    /**
     * @brief TIM12 CH1-CH2 Remapping.
     *
     * By default, TIM12 channels are connected to PC4-PC5. Enabling this remap will connect the channels to PB12-PB13.
     *
     * @note This remap is only available for High Density STM32F100 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR2_TIM12_REMAP.
     */
    kTIM12 = AFIO_MAPR2_TIM12_REMAP,
    /**
     * @brief TIM6-TIM7 DAC to DMA Remapping.
     *
     * By default, TIM6_DAC1/TIM7_DAC2 requests are mapped onto DMA2_CH3/DMA2_CH4. Enabling this remap will map the
     * requests to DMA1_CH3/DMA1_CH4.
     *
     * @note This remap is only available for STM32F100 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR2_TIM76_DAC_DMA_REMAPE.
     */
    kTIM76DMA = AFIO_MAPR2_TIM76_DAC_DMA_REMAPE,
    /**
     * @brief NADV Connection Selection.
     *
     * By default, FSMC_NADV is connected to the output. Enabling this remap will disable this, allowing the pin to be
     * used by another peripheral.
     *
     * @note This remap is only available for STM32F101/102/103/105/107 and High Density STM32F100 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR2_FSMC_NADV_DISCONNECT.
     */
    kDisconnectNADV = AFIO_MAPR2_FSMC_NADV_DISCONNECT,
    /**
     * @brief TIM14 CH1 Remapping.
     *
     * For STM32F100 devices: By default, TIM14_CH1 is connected to PC9. Enabling this remap will connect the channel to
     * PB1.
     *
     * For other STM32F10x devices: By default, TIM14_CH1 is connected to PA7. Enabling this remap will connect the
     * channel to PF9.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR2_TIM14_REMAP.
     */
    kTIM14 = AFIO_MAPR2_TIM14_REMAP,
    /**
     * @brief TIM13 CH1 Remapping.
     *
     * For STM32F100 devices: By default, TIM13_CH1 is connected to PC8. Enabling this remap will connect the channel to
     * PB0.
     *
     * For other STM32F10x devices: By default, TIM13_CH1 is connected to PA6. Enabling this remap will connect the
     * channel to PF8.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR2_TIM13_REMAP.
     */
    kTIM13 = AFIO_MAPR2_TIM13_REMAP,
    /**
     * @brief TIM11 CH1 Remapping.
     *
     * By default, TIM11_CH1 is connected to PB9. Enabling this remap will connect the channel to PF7.
     *
     * @note This remap is only available for STM32F101/102/103/105/107 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR2_TIM11_REMAP.
     */
    kTIM11 = AFIO_MAPR2_TIM11_REMAP,
    /**
     * @brief TIM10 CH1 Remapping.
     *
     * By default, TIM10_CH1 is connected to PB8. Enabling this remap will connect the channel to PF6.
     *
     * @note This remap is only available for STM32F101/102/103/105/107 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR2_TIM10_REMAP.
     */
    kTIM10 = AFIO_MAPR2_TIM10_REMAP,
    /**
     * @brief TIM9 CH1-CH2 Remapping.
     *
     * By default, TIM9 channels are connected to PA2-PA3. Enabling this remap will connect the channels to PE5-PE6.
     *
     * @note This remap is only available for STM32F101/102/103/105/107 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR2_TIM9_REMAP.
     */
    kTIM9 = AFIO_MAPR2_TIM9_REMAP,
    /**
     * @brief TIM1 DMA Remapping.
     *
     * By default, TIM1_CH1/TIM1_CH2 DMA requests are mapped onto DMA1_CH2/DMA1_CH3. Enabling this remap will map the
     * requests to DMA1_CH6.
     *
     * @note This remap is only available for STM32F100 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR2_TIM1_DMA_REMAP.
     */
    kTIM1DMA = AFIO_MAPR2_TIM1_DMA_REMAP,
    /**
     * @brief CEC Remapping.
     *
     * By default, the CEC line is connected to PB8. Enabling this remap will connect the line to PB10.
     *
     * @note This remap is only available for STM32F100 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR2_CEC_REMAP.
     */
    kCEC = AFIO_MAPR2_CEC_REMAP,
    /**
     * @brief TIM17 CH1 Remapping.
     *
     * By default, TIM17_CH1 is connected to PB9. Enabling this remap will connect the channel to PA7.
     *
     * @note This remap is only available for STM32F100 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR2_TIM17_REMAP.
     */
    kTIM17 = AFIO_MAPR2_TIM17_REMAP,
    /**
     * @brief TIM16 CH1 Remapping.
     *
     * By default, TIM16_CH1 is connected to PB8. Enabling this remap will connect the channel to PA6.
     *
     * @note This remap is only available for STM32F100 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR2_TIM16_REMAP.
     */
    kTIM16 = AFIO_MAPR2_TIM16_REMAP,
    /**
     * @brief TIM15 CH1-CH2 Remapping.
     *
     * By default, TIM15 channels are connected to PA2-PA3. Enabling this remap will connect the channels to PB14-PB15.
     *
     * @note This remap is only available for STM32F100 devices.
     *
     * Equivalent to libopencm3 macro @c AFIO_MAPR2_TIM16_REMAP.
     */
    kTIM15 = AFIO_MAPR1_TIM16_REMAP
  };

  /**
   * @brief Configuration for GPIO.
   */
  struct Config {
    /**
     * @brief GPIO Pin to enable and initialize.
     *
     * Defaults to @code {GPIOA, GPIO0} @endcode.
     */
    Pinout pin = {};

    /**
     * @brief GPIO Mode.
     *
     * Defaults to Configuration#kInputFloat.
     */
    Configuration cnf = Configuration::kInputFloat;

    /**
     * @brief GPIO Output Speed.
     *
     * Defaults to Mode#kInput.
     */
    Mode mode = Mode::kInput;
  };

  /**
   * @brief Conversion constructor.
   *
   * @param config Configuration for the GPIO. See GPIO#Config.
   */
  explicit GPIO(const Config& config);

  /**
   * @brief Constructor for internal API use.
   *
   * This constructor is equivalent to @code GPIO::GPIO(const Config&) @endcode. However, it is suggested to use that
   * constructor instead for code readability.
   *
   * @param pin MCU pinout
   * @param cnf GPIO Configuration
   * @param mode GPIO Mode
   */
  GPIO(Pinout pin, Configuration cnf, Mode mode);

  /**
   * Default trivial destructor.
   */
  ~GPIO() = default;

  /**
   * @brief Move constructor.
   *
   * @param other GPIO object to move from
   */
  GPIO(GPIO&& other) noexcept = default;
  /**
   * @brief Move assignment operator.
   *
   * @param other GPIO object to move from
   * @return Reference to the moved GPIO.
   */
  GPIO& operator=(GPIO&& other) noexcept = default;

  /**
   * @brief Copy constructor.
   *
   * This constructor is deleted because there should only be one object managing each GPIO pin, similar to @c
   * std::unique_ptr.
   */
  GPIO(const GPIO&) = delete;
  /**
   * @brief Copy assignment operator.
   *
   * This constructor is deleted because there should only be one object managing each GPIO pin, similar to @c
   * std::unique_ptr.
   */
  GPIO& operator=(const GPIO&) = delete;

  /**
   * @brief Reads the current logic state of the managed GPIO.
   *
   * @return @c true if high value, otherwise @c false
   */
  bool Read() const;
  /**
   * @brief Sets new GPIO state.
   *
   * @param state New state of GPIO, where @c true represents a high value, and @c false represents a low value.
   */
  void Set(bool state) const;
  /**
   * @brief Toggles GPIO state. Logic High -> Logic Low and vice versa.
   */
  void Toggle() const;

  /**
   * @brief Resets the GPIO to its original configuration, i.e. when the RESET button is first pushed.
   */
  void Reset() const;

  /**
   * @brief Configures the primary remap functionality of all GPIOs.
   *
   * Most pins have alternate functions associated with them. This function allows the primary set of these functions
   * to be enabled, along with the option to disable JTAG controls for GPIOs.
   *
   * By default, all alternate functions are disabled.
   *
   * See the @c gpio_primary_remap() section of http://libopencm3.org/docs/latest/stm32f1/html/group__gpio__file.html.
   *
   * @param swj_state Whether to disable JTAG capability
   * @param maps The alternate functions to enable. Use bitwise OR on all libopencm3 constants to enable the remaps with
   * one function call.
   */
  static void SetPriAltFn(JTAGDisables swj_state, AltFnMaps maps);

  /**
   * @brief Configures the secondary remap functionality of all GPIOs.
   *
   * Most pins have alternate functions associated with them. This function allows the primary set of these functions
   * to be enabled, along with the option to disable JTAG controls for GPIOs.
   *
   * See the libopencm3 documentation for the full list of remaps:
   * http://libopencm3.org/docs/latest/stm32f1/html/group__afio__remap2.html
   *
   * See the @c gpio_secondary_remap() section of http://libopencm3.org/docs/latest/stm32f1/html/group__gpio__file.html.
   *
   * @param maps The alternate functions to enable. Use bitwise OR on all libopencm3 constants to enable the remaps with
   * one function call.
   */
  static void SetSecAltFn(AltFnMaps maps);

  /**
   * @brief Configures one primary remap functionality.
   *
   * As opposed to SetPriAltFn(JTAGDisables,AltFnMaps), this function only enables one alternate function from the
   * primary set.
   *
   * @param swj_state Whether to disable JTAG capability
   * @param map The alternate function to enable. See GPIO#PriRemap for a comprehensive list of primary alternate
   * functions.
   */
  static void SetPriAltFn(JTAGDisables swj_state, PriRemap map) { SetPriAltFn(swj_state, static_cast<uint32_t>(map)); }

  /**
 * @brief Configures one secondary remap functionality.
 *
 * As opposed to SetSecAltFn(AltFnMaps), this function only enables one alternate function from the secondary set.
 *
 * @param map The alternate function to enable. See GPIO#SecRemap for a comprehensive list of secondary alternate
   * functions.
 */
  static void SetSecAltFn(SecRemap map) { SetSecAltFn(static_cast<uint32_t>(map)); }

 private:
  /**
 * @brief Initializes this GPIO to the given configuration for STM32F1xx devices.
 *
 * See GPIO#Config for what @p mode and @p speed means.
 *
 * @param cnf GPIO Configuration
 * @param mode GPIO Mode
 */
  void Init(Configuration cnf, Mode mode) const;

  /**
   * @brief Initializes an RCC Clock according to which port is initialized.
   *
   * This function is a simple helper function to determine which RCC to initialize, given the port we are working with.
   *
   * @param port The GPIO port which should be initialized
   */
  void InitRcc(Port port) const;

  /**
   * @brief Retrieves the GPIO pinout currently managed by this object.
   *
   * @warning Do not attempt to modify this reference. Doing so may break all functions for this pin.
   *
   * @return The MCU pinout of the currently managed GPIO pin.
   */
  const Pinout& GetPinout() const { return pin_; }
  /**
   * @brief MCU Pinout of the currently managed GPIO.
   */
  Pinout pin_;
};

}  // namespace stm32f1
}  // namespace core

#endif  // defined(STM32F1)

#endif  // RTLIB_CORE_STM32F1_GPIO_H_
