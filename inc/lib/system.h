#ifndef RTLIB_LIB_SYSTEM_H_
#define RTLIB_LIB_SYSTEM_H_

#include <cstdint>

class System {
 public:
  static void Init();

  static uint32_t GetMs();
};

#endif  // RTLIB_LIB_SYSTEM_H_
