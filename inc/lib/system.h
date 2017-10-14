#ifndef RTLIB_LIB_SYSTEM_H_
#define RTLIB_LIB_SYSTEM_H_

#include <cstdint>

/**
 * @brief HAL implementation for system clock.
 *
 * This abstraction layer provides timekeeping functionality. Since there is only one system clock, member functions
 * can be called directly.
 *
 * To use this class, Init() must be called. Otherwise, any of the member functions will refuse to work.
 */
class System {
 public:
  /**
   * @brief Enumeration of clock resolution.
   *
   * Generally, System::kStdRes will be used for its balance between accuracy and performance. Other accuracies
   * are only for testing purposes.
   */
  enum ClockResolution : uint32_t {
    /**
     * @brief Low Clock Resolution.
     *
     * %System clock will update once every 1 second.
     */
        kLowRes = 1,
    /**
     * @brief Standard Clock Resolution.
     *
     * %System clock will update once every 1 millisecond.
     */
        kStdRes = 1000,
    /**
     * @brief High Clock Resolution.
     *
     * %System clock will update once every 1 microsecond.
     */
        kHighRes = 1000000
  };

  /**
   * @brief Initializes the timekeeping functionality.
   *
   * @warning This function should be invoked first before using other auxiliary functions. Failure to do so will
   * cause incorrect times to be reported, and infinitely delaying loops.
   *
   * @param clock_res Resolution of the clock. Defaults to 1 ms/update.
   */
  static void Init(ClockResolution clock_res = kStdRes);

  /**
   * @brief Returns the time since clock initialization.
   *
   * It is recommended to use this function if you use a high resolution clock.
   *
   * @note If using standard or low resolution clocks, the values will be clamped down to the nearest 1000us (for
   * standard resolution) or 1000000us (for low resolution).
   *
   * @return Microseconds since clock initialization.
   */
  static uint64_t GetUs();
  /**
   * @brief Returns the time since clock initialization.
   *
   * It is recommended to use this function if you use a standard resolution clock.
   *
   * @note If using high or low resolution clock, the values will be clamped down to the nearest 1ms (for high
   * resolution) or 1000ms (for low resolution).
   *
   * @return Milliseconds since clock initialization.
   */
  static uint64_t GetMs();
  /**
   * @brief Returns the time since clock initialization.
   *
   * It is recommended to use this function if you use a low resolution clock.
   *
   * @note If using standard or high resolution clock, the values will be clamped down to the nearest 1s.
   *
   * @return Seconds since clock initialization.
   */
  static uint64_t GetS();

  /**
   * @brief Temporarily halt program execution.
   *
   * It is recommended to use this function if you use a high resolution clock.
   *
   * @note If using standard or low resolution clock, the wait time will be rounded to the nearest 1ms (for standard
   * resolution) or 1s (for low resolution).
   *
   * @param wait_us Microseconds to wait.
   */
  static void DelayUs(uint64_t wait_us);
  /**
   * @brief Temporarily halt program execution.
   *
   * It is recommended to use this function if you use a standard resolution clock.
   *
   * @note If using low resolution clock, the wait time will be rounded to the nearest 1s.
   *
   * @param wait_ms Milliseconds to wait.
   */
  static void DelayMs(uint64_t wait_ms);
  /**
   * @brief Temporarily halt program execution.
   *
   * It is recommended to use this function if you use a low resolution clock.
   *
   * @param wait_s Seconds to wait.
   */
  static void DelayS(uint64_t wait_s);

 private:
  static ClockResolution clock_res_;
};

#endif  // RTLIB_LIB_SYSTEM_H_
