#ifndef _DEMO_NET_LOOP_H_
#define _DEMO_NET_LOOP_H_

#include <sys/epoll.h>
#include <map>
#include <vector>

namespace demo {

namespace net {

class Channel;

class Poller {
  typedef std::map<int, Channel*> ChannelMap;
  typedef std::vector<struct epoll_event> EventList;

 public:
  Poller();

  ~Poller();

  void Dispatch(int timeout_ms);

  void AddChannel(Channel* chn);

  void DelChannel(Channel* chn);

 private:
  void updateChannel(int operation, Channel* chn);

  const char* toString(int operation);

 private:
  int epollfd_;
  ChannelMap channels_;
  EventList events_;
};

}  // namespace net

}  // namespace demo

#endif  // _DEMO_NET_LOOP_H_