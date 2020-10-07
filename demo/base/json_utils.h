#ifndef _DEMO_BASE_JSON_UTILS_H_
#define _DEMO_BASE_JSON_UTILS_H_

#include <stdint.h>
#include <string>
#include "json/json.h"

namespace demo {

class JsonUtils {
 public:
  static bool ParseJsonContext(const char* context, uint32_t len,
                               Json::Value& value);

  static bool IsObjectMember(const Json::Value& value, const char* key);

  static bool IsArrayMember(const Json::Value& value, const char* key);

  static bool IsStringMember(const Json::Value& value, const char* key);

  static bool IsBoolMember(const Json::Value& value, const char* key);

  static bool IsIntMember(const Json::Value& value, const char* key);

  static bool IsUIntMember(const Json::Value& value, const char* key);

  static std::string AsString(const Json::Value& value, const char* key);

  static const char* AsCString(const Json::Value& value, const char* key);

  static int32_t AsInt(const Json::Value& value, const char* key,
                       int32_t ret = -1);

  static uint32_t AsUInt(const Json::Value& value, const char* key,
                         uint32_t ret = 0);

  static bool AsBool(const Json::Value& value, const char* key,
                     bool ret = false);

  static std::string ToString(const Json::Value& value);

 private:
  static Json::Reader reader;
  static Json::FastWriter writer;
};

}  // namespace demo

#endif  // _DEMO_BASE_JSON_UTILS_H_