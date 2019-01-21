#pragma once

#if defined(STM32F1)

#include <cstdint>

#include "core/stm32/f1/memory_map.h"

namespace rtlib::core::stm32::f1 {

/**
 * @brief RCC peripherals enumeration.
 */
enum struct rcc_periph {
  // APB2
  afio,
  iopa,
  iopb,
  iopc,
  iopd,
  iope,
  iopf,
  iopg,
  adc1,
  adc2,
  tim1,
  spi1,
  tim8,
  usart1,
  adc3,
  tim9,
  tim10,
  tim11,

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
  usb,
  can,
  can1,
  can2,
  bkp,
  pwr,
  dac,

  // AHB
  dma1,
  dma2,
  sram,
  flit,
  crc,
  fsmc,
  sdio,
  otg_fs,
  eth_mac,
  eth_mac_tx,
  eth_mac_rx
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
   * @return The address of the clock configuration register (CFGR).
   */
  static constexpr std::uint32_t cfgr() { return addr() + 0x04; }
  /**
   * @return The address of the clock interrupt register (CIR).
   */
  static constexpr std::uint32_t cir() { return addr() + 0x08; }

  /**
   * @return The address of the APB2 peripheral clock reset register (APB2RSTR).
   */
  static constexpr std::uint32_t apb2rstr() { return addr() + 0x0C; }
  /**
   * @return The address of the APB1 peripheral clock reset register (APB1RSTR).
   */
  static constexpr std::uint32_t apb1rstr() { return addr() + 0x10; }

  /**
   * @return The address of the AHB peripheral clock enable register (AHBENR).
   */
  static constexpr std::uint32_t ahbenr() { return addr() + 0x14; }
  /**
   * @return The address of the APB2 peripheral clock enable register (APB2ENR).
   */
  static constexpr std::uint32_t apb2enr() { return addr() + 0x18; }
  /**
   * @return The address of the APB1 peripheral clock enable register (APB1ENR).
   */
  static constexpr std::uint32_t apb1enr() { return addr() + 0x1C; }

  /**
   * @return The address of the backup domain control register (BDCR).
   */
  static constexpr std::uint32_t bdcr() { return addr() + 0x20; }
  /**
   * @return The address of the clock control & status register (CSR).
   */
  static constexpr std::uint32_t csr() { return addr() + 0x24; }

  /**
   * @return The address of the AHB peripheral clock reset register (AHBRRSTR).
   */
  static constexpr std::uint32_t ahbstr() { return addr() + 0x28; }

  /**
   * @return The address of the clock configuration register2 (CFGR2).
   */
  static constexpr std::uint32_t cfgr2() { return addr() + 0x2C; }
};

template<>
struct rcc_map<rcc_periph::iopa> {
  static constexpr std::uint32_t reset = rcc::apb2rstr();
  static constexpr std::uint32_t enable = rcc::apb2enr();
  static constexpr std::uint8_t offset = 2;
};

template<>
struct rcc_map<rcc_periph::iopb> {
  static constexpr std::uint32_t reset = rcc::apb2rstr();
  static constexpr std::uint32_t enable = rcc::apb2enr();
  static constexpr std::uint8_t offset = 3;
};

template<>
struct rcc_map<rcc_periph::iopc> {
  static constexpr std::uint32_t reset = rcc::apb2rstr();
  static constexpr std::uint32_t enable = rcc::apb2enr();
  static constexpr std::uint8_t offset = 4;
};

template<>
struct rcc_map<rcc_periph::iopd> {
  static constexpr std::uint32_t reset = rcc::apb2rstr();
  static constexpr std::uint32_t enable = rcc::apb2enr();
  static constexpr std::uint8_t offset = 5;
};

template<>
struct rcc_map<rcc_periph::iope> {
  static constexpr std::uint32_t reset = rcc::apb2rstr();
  static constexpr std::uint32_t enable = rcc::apb2enr();
  static constexpr std::uint8_t offset = 6;
};

template<>
struct rcc_map<rcc_periph::iopf> {
  static constexpr std::uint32_t reset = rcc::apb2rstr();
  static constexpr std::uint32_t enable = rcc::apb2enr();
  static constexpr std::uint8_t offset = 7;
};

template<>
struct rcc_map<rcc_periph::iopg> {
  static constexpr std::uint32_t reset = rcc::apb2rstr();
  static constexpr std::uint32_t enable = rcc::apb2enr();
  static constexpr std::uint8_t offset = 8;
};
}  // namespace rtlib::core::stm32::f1

#endif  // defined(STM32F1)