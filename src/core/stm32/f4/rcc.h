#pragma once

#if defined(STM32F4)

#include <cstdint>

#include "core/stm32/f4/memory_map.h"

namespace rtlib::core::stm32::f4 {

/**
 * @brief RCC peripherals enumeration.
 */
enum struct rcc_periph {
  // AHB1
  gpioa,
  gpiob,
  gpioc,
  gpiod,
  gpioe,
  gpiof,
  gpiog,
  gpioh,
  gpioi,
  gpioj,
  gpiok,
  crc,
  dma1,
  dma2,
  dma2d,
  eth_mac,
  otg_hs,

  // AHB2
  dcmi,
  cryp,
  hash,
  rng,
  otg_fs,

  // AHB3
  fsmc,
  fmc,

  // APB1
  tim2,
  tim3,
  tim4,
  tim5,
  tim6,
  tim7,
  tim12,
  tim13,
  tim14,
  wwdg,
  spi2,
  spi3,
  usart2,
  usart3,
  uart4,
  uart5,
  i2c1,
  i2c2,
  i2c3,
  can1,
  can2,
  pwr,
  dac,
  uart7,
  uart8,

  // APB2
  tim1,
  tim8,
  usart1,
  usart6,
  adc1,
  adc2,
  adc3,
  adc,
  sdio,
  spi1,
  spi4_5,
  syscfg,
  tim9,
  tim10,
  tim11,
  spi5,
  spi6,
  sai1,
  ltdc,
};

template<rcc_periph P>
struct rcc_map;

/**
 * @brief RCC abstraction layer.
 */
class rcc {
 public:
  rcc() = delete;
  rcc(const rcc&) = delete;
  rcc(rcc&&) = delete;
  rcc& operator=(const rcc&) = delete;
  rcc& operator=(rcc&&) = delete;

  /**
   * @brief Enables the clock for a peripheral.
   *
   * @tparam P Peripheral clock to enable.
   */
  template<rcc_periph P>
  static void enable_peripheral() {
    constexpr std::uint32_t addr = rcc_map<P>::enable;
    constexpr std::uint8_t offset = rcc_map<P>::offset;

    auto val = get_mem(addr);
    val = reset_and_set(val, offset, 1, std::uint32_t(1));
    set_mem(addr, val);
  }

  /**
   * @brief Resets the clock for a peripheral.
   *
   * @tparam P Peripheral clock to reset.
   */
  template<rcc_periph P>
  static void reset_peripheral() {
    constexpr std::uint32_t addr = rcc_map<P>::reset;
    constexpr std::uint8_t offset = rcc_map<P>::offset;

    auto val = get_mem(addr);
    val = reset_and_set(val, offset, 1, std::uint32_t(1));
    set_mem(addr, val);
  }

 private:
  template<rcc_periph P>
  friend struct rcc_map;

  /**
   * @return The base memory boundary of RCC registers.
   */
  static constexpr std::uint32_t addr() { return memory_map<reg::periph_rcc>::begin; }

  /**
   * @return The address of the clock control register (CR).
   */
  static constexpr std::uint32_t cr() { return addr() + 0x00; }
  /**
   * @return The address of the PLL configuration register (PLLCFGR).
   */
  static constexpr std::uint32_t pllcfgr() { return addr() + 0x04; }
  /**
   * @return The address of the clock configuration register (CFGR).
   */
  static constexpr std::uint32_t cfgr() { return addr() + 0x08; }
  /**
   * @return The address of the clock interrupt register (CIR).
   */
  static constexpr std::uint32_t cir() { return addr() + 0x0C; }

  /**
   * @return The address of the AHB1 peripheral clock reset register (AHB1RSTR).
   */
  static constexpr std::uint32_t ahb1rstr() { return addr() + 0x10; }
  /**
   * @return The address of the AHB2 peripheral clock reset register (AHB2RSTR).
   */
  static constexpr std::uint32_t ahb2rstr() { return addr() + 0x14; }
  /**
   * @return The address of the AHB3 peripheral clock reset register (AHB3RSTR).
   */
  static constexpr std::uint32_t ahb3rstr() { return addr() + 0x18; }
  /**
   * @return The address of the APB1 peripheral clock reset register (APB1RSTR).
   */
  static constexpr std::uint32_t apb1rstr() { return addr() + 0x20; }
  /**
   * @return The address of the APB2 peripheral clock reset register (APB2RSTR).
   */
  static constexpr std::uint32_t apb2rstr() { return addr() + 0x24; }

  /**
   * @return The address of the AHB1 peripheral clock enable register (AHB1ENR).
   */
  static constexpr std::uint32_t ahb1enr() { return addr() + 0x30; }
  /**
   * @return The address of the AHB2 peripheral clock enable register (AHB2ENR).
   */
  static constexpr std::uint32_t ahb2enr() { return addr() + 0x34; }
  /**
   * @return The address of the AHB3 peripheral clock enable register (AHB3ENR).
   */
  static constexpr std::uint32_t ahb3enr() { return addr() + 0x38; }
  /**
   * @return The address of the APB1 peripheral clock enable register (APB1ENR).
   */
  static constexpr std::uint32_t apb1enr() { return addr() + 0x40; }
  /**
   * @return The address of the APB2 peripheral clock enable register (APB2ENR).
   */
  static constexpr std::uint32_t apb2enr() { return addr() + 0x44; }

  /**
   * @return The address of the AHB1 peripheral clock enable in low power register (AHB1LPENR).
   */
  static constexpr std::uint32_t ahb1lpenr() { return addr() + 0x50; }
  /**
   * @return The address of the AHB2 peripheral clock enable in low power register (AHB2LPENR).
   */
  static constexpr std::uint32_t ahb2lpenr() { return addr() + 0x54; }
  /**
   * @return The address of the AHB3 peripheral clock enable in low power register (AHB3LPENR).
   */
  static constexpr std::uint32_t ahb3lpenr() { return addr() + 0x58; }
  /**
   * @return The address of the APB1 peripheral clock enable in low power register (APB1LPENR).
   */
  static constexpr std::uint32_t apb1lpenr() { return addr() + 0x60; }
  /**
   * @return The address of the APB2 peripheral clock enable in low power register (AHB2LPENR).
   */
  static constexpr std::uint32_t apb2lpenr() { return addr() + 0x64; }

  /**
   * @return The address of the backup domain control register (BDCR).
   */
  static constexpr std::uint32_t bdcr() { return addr() + 0x70; }
  /**
   * @return The address of the clock control & status register (CSR).
   */
  static constexpr std::uint32_t csr() { return addr() + 0x74; }

  /**
   * @return The address of the spread spectrum clock generation register (SSCGR).
   */
  static constexpr std::uint32_t sscgr() { return addr() + 0x80; }
  /**
   * @return The address of the PLLI2S configuration register (PLLI2SCFGR).
   */
  static constexpr std::uint32_t plli2scfgr() { return addr() + 0x84; }
  /**
   * @return The address of the PLL configuration register (PLLSAICFGR).
   */
  static constexpr std::uint32_t pllsaicfgr() { return addr() + 0x88; }
  /**
   * @return The address of the dedicated clock configuration register (DCKCFGR).
   */
  static constexpr std::uint32_t dckcfgr() { return addr() + 0x8C; }
};

template<>
struct rcc_map<rcc_periph::gpioa> {
  static constexpr std::uint32_t reset = rcc::ahb1rstr();
  static constexpr std::uint32_t enable = rcc::ahb1enr();
  static constexpr std::uint8_t offset = 0;
};

template<>
struct rcc_map<rcc_periph::gpiob> {
  static constexpr std::uint32_t reset = rcc::ahb1rstr();
  static constexpr std::uint32_t enable = rcc::ahb1enr();
  static constexpr std::uint8_t offset = 1;
};

template<>
struct rcc_map<rcc_periph::gpioc> {
  static constexpr std::uint32_t reset = rcc::ahb1rstr();
  static constexpr std::uint32_t enable = rcc::ahb1enr();
  static constexpr std::uint8_t offset = 2;
};

template<>
struct rcc_map<rcc_periph::gpiod> {
  static constexpr std::uint32_t reset = rcc::ahb1rstr();
  static constexpr std::uint32_t enable = rcc::ahb1enr();
  static constexpr std::uint8_t offset = 3;
};

template<>
struct rcc_map<rcc_periph::gpioe> {
  static constexpr std::uint32_t reset = rcc::ahb1rstr();
  static constexpr std::uint32_t enable = rcc::ahb1enr();
  static constexpr std::uint8_t offset = 4;
};

template<>
struct rcc_map<rcc_periph::gpiof> {
  static constexpr std::uint32_t reset = rcc::ahb1rstr();
  static constexpr std::uint32_t enable = rcc::ahb1enr();
  static constexpr std::uint8_t offset = 5;
};

template<>
struct rcc_map<rcc_periph::gpiog> {
  static constexpr std::uint32_t reset = rcc::ahb1rstr();
  static constexpr std::uint32_t enable = rcc::ahb1enr();
  static constexpr std::uint8_t offset = 6;
};

template<>
struct rcc_map<rcc_periph::gpioh> {
  static constexpr std::uint32_t reset = rcc::ahb1rstr();
  static constexpr std::uint32_t enable = rcc::ahb1enr();
  static constexpr std::uint8_t offset = 7;
};

template<>
struct rcc_map<rcc_periph::gpioi> {
  static constexpr std::uint32_t reset = rcc::ahb1rstr();
  static constexpr std::uint32_t enable = rcc::ahb1enr();
  static constexpr std::uint8_t offset = 8;
};
}  // namespace rtlib::core::stm32::f4

#endif  // defined(STM32F4)