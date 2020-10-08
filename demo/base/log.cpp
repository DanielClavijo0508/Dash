#include "/data/Dash/demo/base/log.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

namespace demo {

namespace base {

static const char* baseFilename(const char* file) {
  const char* base = strrchr(file, '/');
  if (base && base[1] != '\0') {
    return base + 1;
  }
  return file;
}

int Log::sLevel_ = Log::LEVEL_ERROR;

void Log::log(const char* level, const char* file, const char* func, int line,
              const char* fmt, ...) {
  va_list args;
  printf("[%s:%s:%d][%s]", baseFilename(file), func, line, level);
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  printf("\n");
}

}  // namespace base

}  // namespace demo
