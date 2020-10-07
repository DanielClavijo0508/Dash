#ifndef _DEMO_NET_N9M_PROTOCOL_H_
#define _DEMO_NET_N9M_PROTOCOL_H_

#include <stdint.h>

namespace demo {

namespace net {

#pragma pack(1)
struct N9mProtocol {
  uint32_t V : 2;
  uint32_t P : 1;
  uint32_t M : 1;
  uint32_t CSRC : 4;
  uint32_t PT : 8;
  uint32_t SSRC : 16;
  uint32_t counts;
  uint32_t reserve;
};
#pragma pack()

enum eN9mPayloadType {
  PT_SIGNAL = 0,
};

}  // namespace net

}  // namespace demo

#endif  // _DEMO_NET_N9M_PROTOCOL_H_