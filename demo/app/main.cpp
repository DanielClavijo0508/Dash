#include "/data/Dash/demo/app/p2_client.h"
#include "/data/Dash/demo/base/log.h"
#include "/data/Dash/demo/net/poller.h"

using demo::app::P2Client;
using demo::net::Poller;

#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include <list>

typedef std::list<P2Client*> ClientList;

void SignalHandler(int sig) {
  // TODO:
}

bool IsValidIpv4Addr(const char* ip) {
  struct sockaddr_in addr;
  return (1 == ::inet_pton(AF_INET, ip, &addr));
}

void DispatchClient(ClientList& clients) {
  ClientList::iterator it = clients.begin();
  while (it != clients.end()) {
    if (*it) {
      (*it)->Handle();
    }
    ++it;
  }
}

int main(int argc, char** argv) {
  printf("---------------\n");
  printf("p2 client demo\n");
  printf("%s %s\n", __DATE__, __TIME__);
  printf("---------------\n");

  Log::set_level(Log::LEVEL_INFO);

  if (argc < 2) {
    LOG_ERROR("Usage: %s ip1 ip2 ...", argv[0]);
    return -1;
  }

  signal(SIGPIPE, SignalHandler);

  Poller poller;

  LOG_INFO("start initialize p2 clients.");

  ClientList clients;
  for (int i = 1; i < argc; ++i) {
    if (IsValidIpv4Addr(argv[i])) {
      P2Client* c = new (std::nothrow) P2Client(&poller, argv[i]);

      if (c && 0 == c->Initialize()) {
        LOG_INFO("add %s success.", c->name());
        clients.push_back(c);
      } else {
        LOG_ERROR("add p2client with %s failed.", argv[i]);
        if (c) delete c;
      }
    }
  }

  LOG_INFO("finish initialize p2 clients size %d.", clients.size());

  while (1) {
    poller.Dispatch(50);
    DispatchClient(clients);
  }

  return 0;
}
