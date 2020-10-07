#ifndef _DEMO_NET_SOCKET_UTILS_H_
#define _DEMO_NET_SOCKET_UTILS_H_

#include <arpa/inet.h>

namespace demo {

namespace net {

class SocketUtils {
 public:
  static int CreateNonBlockOrDie(sa_family_t family);

  static int Connect(int fd, const struct sockaddr* addr);

  static ssize_t Read(int fd, void* buf, size_t count);

  static ssize_t Write(int fd, void* buf, size_t count);

  static void Close(int fd);

  static bool BindToDevice(int fd, const char* dev);

  static int GetSocketErr(int fd);
};

}  // namespace net

}  // namespace demo

#endif  // _DEMO_NET_SOCKET_UTILS_H_