#include "/data/Dash/demo/base/common_utils.h"

#include <stdio.h>

namespace demo {

namespace base {

void CommonUtils::Hexdump(const char* data, uint32_t size) {
  unsigned short line = 0;
  while (size > 0) {
    unsigned olen = size < 16 ? size : 16;

    printf("%04X: ", line++);
    for (unsigned i = 0; i < olen; ++i) printf("%02X ", data[i]);

    for (unsigned i = 0; i < (16 - olen); ++i) printf("   ");
    printf("   ");

    for (unsigned i = 0; i < olen; ++i)
      printf("%c", data[i] >= 32 && data[i] < 127 ? data[i] : '.');
    printf("\n");

    data += olen;
    size -= olen;
  }
}

}  // namespace base

}  // namespace demo
