#ifndef _DEMO_BASE_COMMON_UTILS_H_
#define _DEMO_BASE_COMMON_UTILS_H_

#include <stdint.h>

namespace demo {

namespace base {

class CommonUtils {
 public:
  static void Hexdump(const char* data, uint32_t size);
};

}  // namespace base

}  // namespace demo

#endif  // _DEMO_BASE_COMMON_UTILS_H_