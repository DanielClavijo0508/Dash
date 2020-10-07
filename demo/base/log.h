#ifndef _DEMO_BASE_LOG_H_
#define _DEMO_BASE_LOG_H_

#include <stdlib.h>

namespace demo {

namespace base {

class Log {
 public:
  enum eLogLevel {
    LEVEL_FATAL = 0,
    LEVEL_ERROR,
    LEVEL_WARN,
    LEVEL_INFO,
    LEVEL_TRACE
  };

 public:
  static void log(const char* level, const char* file, const char* func,
                  int line, const char* fmt, ...);

  static void set_level(int lv) { sLevel_ = lv; }

  static int level() { return sLevel_; }

 private:
  static int sLevel_;
};

}  // namespace base

}  // namespace demo

using demo::base::Log;

#define LOG_FATAL(fmt...)                                       \
  do {                                                          \
    if (Log::level() >= Log::LEVEL_FATAL)                       \
      Log::log("FATAL", __FILE__, __FUNCTION__, __LINE__, fmt); \
    exit(1);                                                    \
  } while (0)

#define LOG_ERROR(fmt...)                                       \
  do {                                                          \
    if (Log::level() >= Log::LEVEL_ERROR)                       \
      Log::log("ERROR", __FILE__, __FUNCTION__, __LINE__, fmt); \
  } while (0)

#define LOG_WARN(fmt...)                                       \
  do {                                                         \
    if (Log::level() >= Log::LEVEL_WARN)                       \
      Log::log("WARN", __FILE__, __FUNCTION__, __LINE__, fmt); \
  } while (0)

#define LOG_INFO(fmt...)                                       \
  do {                                                         \
    if (Log::level() >= Log::LEVEL_INFO)                       \
      Log::log("INFO", __FILE__, __FUNCTION__, __LINE__, fmt); \
  } while (0)

#define LOG_TRACE(fmt...)                                       \
  do {                                                          \
    if (Log::level() >= Log::LEVEL_TRACE)                       \
      Log::log("TRACE", __FILE__, __FUNCTION__, __LINE__, fmt); \
  } while (0)

#endif  // _DEMO_BASE_LOG_H_
