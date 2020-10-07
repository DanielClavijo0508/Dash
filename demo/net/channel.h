#ifndef _DEMO_NET_CHANNEL_H_
#define _DEMO_NET_CHANNEL_H_

#include <poll.h>
#include <string>
using std::string;

namespace demo {

namespace net {

class Channel {
 public:
  enum eEvent { E_NONE = 0, E_READ = POLLIN | POLLPRI, E_WRITE = POLLOUT };

 public:
  Channel() : fd_(-1), events_(0), revents_(0) {}

  virtual ~Channel() {}

  void HandleEvent();

  int fd() const { return fd_; }

  int events() const { return events_; }

  void set_revents(int revt) { revents_ = revt; }

 protected:
  void set_fd(int sockfd) { fd_ = sockfd; }

  void set_events(int events) { events_ = events; }

  virtual void OnRead() {}

  virtual void OnWrite() {}

  virtual void OnError() {}

  virtual void OnClose() {}

  string toString(int ev);

 protected:
  int fd_;
  int events_;
  int revents_;
};

}  // namespace net

}  // namespace demo

#endif  // _DEMO_NET_CHANNEL_H_