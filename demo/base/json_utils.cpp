#include "base/json_utils.h"

namespace demo {

Json::Reader JsonUtils::reader;
Json::FastWriter JsonUtils::writer;

bool JsonUtils::ParseJsonContext(const char* context, uint32_t len,
                                 Json::Value& value) {
  return reader.parse(context, context + len, value);
}

bool JsonUtils::IsObjectMember(const Json::Value& value, const char* key) {
  return (value.isMember(key) && value[key].isObject());
}

bool JsonUtils::IsArrayMember(const Json::Value& value, const char* key) {
  return (value.isMember(key) && value[key].isArray());
}

bool JsonUtils::IsStringMember(const Json::Value& value, const char* key) {
  return (value.isMember(key) && value[key].isString());
}

bool JsonUtils::IsBoolMember(const Json::Value& value, const char* key) {
  return (value.isMember(key) && value[key].isBool());
}

bool JsonUtils::IsIntMember(const Json::Value& value, const char* key) {
  return (value.isMember(key) && value[key].isInt());
}

bool JsonUtils::IsUIntMember(const Json::Value& value, const char* key) {
  return (value.isMember(key) && value[key].isUInt());
}

std::string JsonUtils::AsString(const Json::Value& value, const char* key) {
  return IsStringMember(value, key) ? value[key].asString() : "";
}

const char* JsonUtils::AsCString(const Json::Value& value, const char* key) {
  return IsStringMember(value, key) ? value[key].asCString() : "";
}

int32_t JsonUtils::AsInt(const Json::Value& value, const char* key,
                         int32_t ret) {
  return IsIntMember(value, key) ? value[key].asInt() : ret;
}

uint32_t JsonUtils::AsUInt(const Json::Value& value, const char* key,
                           uint32_t ret) {
  return IsUIntMember(value, key) ? value[key].asUInt() : ret;
}

bool JsonUtils::AsBool(const Json::Value& value, const char* key, bool ret) {
  return IsBoolMember(value, key) ? value[key].asBool() : ret;
}

std::string JsonUtils::ToString(const Json::Value& value) {
  return writer.write(value);
}

}  // namespace demo