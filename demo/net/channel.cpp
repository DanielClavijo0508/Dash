#include "net/channel.h"
#include <sstream>
#include "base/log.h"

using namespace demo::base;

namespace demo {

namespace net {

void Channel::HandleEvent() {
  LOG_TRACE("%s\n", toString(revents_).c_str());

  if ((revents_ & POLLHUP) && !(revents_ & POLLIN)) {
    OnClose();
  }

  if (revents_ & (POLLERR | POLLNVAL)) {
    OnError();
  }

  if (revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
    OnRead();
  }

  if (revents_ & POLLOUT) {
    OnWrite();
  }
}

string Channel::toString(int ev) {
  std::ostringstream oss;
  oss << fd_ << ": ";
  if (ev & POLLIN) oss << "IN ";
  if (ev & POLLPRI) oss << "PRI ";
  if (ev & POLLOUT) oss << "OUT ";
  if (ev & POLLHUP) oss << "HUP ";
  if (ev & POLLRDHUP) oss << "RDHUP ";
  if (ev & POLLERR) oss << "ERR ";
  if (ev & POLLNVAL) oss << "NVAL ";

  return oss.str().c_str();
}

}  // namespace net

}  // namespace demo