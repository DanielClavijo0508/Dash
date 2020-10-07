#include "net/poller.h"
#include "base/log.h"
#include "net/channel.h"
using namespace demo::base;

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

namespace demo {

namespace net {

Poller::Poller() : epollfd_(::epoll_create1(EPOLL_CLOEXEC)), events_(8) {
  assert(epollfd_ >= 0);
}

Poller::~Poller() { ::close(epollfd_); }

void Poller::AddChannel(Channel* chn) {
  int fd = chn->fd();
  if (channels_.find(fd) == channels_.end())  // add new
  {
    updateChannel(EPOLL_CTL_ADD, chn);
    channels_[fd] = chn;
  } else  // update channel
  {
    updateChannel(EPOLL_CTL_MOD, chn);
  }
}

void Poller::DelChannel(Channel* chn) {
  int fd = chn->fd();
  assert(channels_.find(fd) != channels_.end());
  assert(channels_[fd] == chn);

  updateChannel(EPOLL_CTL_DEL, chn);
  assert(1 == channels_.erase(fd));
}

void Poller::updateChannel(int operation, Channel* chn) {
  struct epoll_event event;
  bzero(&event, sizeof(event));
  event.events = chn->events();
  event.data.ptr = chn;

  if (::epoll_ctl(epollfd_, operation, chn->fd(), &event) < 0) {
    LOG_ERROR("epoll_wait error: op[%s]fd[%d]\n", toString(operation),
              chn->fd());
    assert(EPOLL_CTL_DEL != operation);
  }
}

void Poller::Dispatch(int timeout_ms) {
  int num_events = ::epoll_wait(epollfd_, &*events_.begin(),
                                static_cast<int>(events_.size()), timeout_ms);

  if (num_events > 0) {
    for (int i = 0; i < num_events; ++i) {
      Channel* active_chn = static_cast<Channel*>(events_[i].data.ptr);
      active_chn->set_revents(events_[i].events);
      active_chn->HandleEvent();
    }

    if (num_events == events_.size()) {
      events_.resize(events_.size() * 2);
    }
  } else if (0 == num_events) {
    // nothing happened
  } else {
    assert(errno == EINTR);
  }
}

const char* Poller::toString(int operation) {
  switch (operation) {
    case EPOLL_CTL_ADD:
      return "ADD";
    case EPOLL_CTL_DEL:
      return "DEL";
    case EPOLL_CTL_MOD:
      return "MOD";
    default:
      assert(false && "ERROR operation");
      return "Unknown Operation";
  }
}

}  // namespace net

}  // namespace demo
