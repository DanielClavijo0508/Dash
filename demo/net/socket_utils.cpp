#include "net/socket_utils.h"
#include "base/log.h"

using namespace demo::base;

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <net/if.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

namespace demo {

namespace net {

int SocketUtils::CreateNonBlockOrDie(sa_family_t family) {
  int sockfd =
      ::socket(family, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
  assert(sockfd >= 0);
  return sockfd;
}

int SocketUtils::Connect(int fd, const struct sockaddr* addr) {
  return ::connect(fd, addr, static_cast<socklen_t>(sizeof(struct sockaddr)));
}

ssize_t SocketUtils::Read(int fd, void* buf, size_t count) {
  return ::read(fd, buf, count);
}

ssize_t SocketUtils::Write(int fd, void* buf, size_t count) {
  return ::write(fd, buf, count);
}

void SocketUtils::Close(int fd) { ::close(fd); }

bool SocketUtils::BindToDevice(int fd, const char* dev) {
  struct ifreq ireq;
  strncpy(ireq.ifr_ifrn.ifrn_name, dev, IFNAMSIZ);
  if (setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, (char*)&ireq, sizeof(ireq)) <
      0) {
    return false;
  }
  return true;
}

int SocketUtils::GetSocketErr(int fd) {
  int optval;
  socklen_t optlen = static_cast<socklen_t>(sizeof optval);

  if (::getsockopt(fd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0) {
    return errno;
  } else {
    return optval;
  }
}

}  // namespace net

}  // namespace demo
