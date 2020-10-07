#ifndef _DEMO_APP_P2_CLIENT_H_
#define _DEMO_APP_P2_CLIENT_H_

#include "net/channel.h"
#include "net/poller.h"
using demo::net::Poller;

#include "base/json_utils.h"

#include <stdint.h>
#include <deque>
#include <map>
#include <string>
using std::string;

namespace demo {

namespace app {

#define DEFAULT_PEER_PORT 9006
#define RECV_BUFLEN 5120

class P2Client : public net::Channel {
  enum eLinkState {
    kLinkDisconn = 0,
    kLinkConnect,
    kLinkVerify,
    kLinkLogin,
    kLinkAlive,
    kLinkTotal
  };

  enum eConnState { kDisconnected, kConnecting, kConnected };

  typedef eLinkState StateId;
  typedef void (P2Client::*FsmHandler)();
  struct FsmState {
    StateId id;
    const char* name;
    FsmHandler onState;
    FsmHandler onEnter;
    FsmHandler onExit;
  };

  struct Buffer {
    char* data;
    uint32_t msn;
    uint32_t len;
    uint32_t snd;

    Buffer(uint32_t sn, char* d, uint32_t l)
        : msn(sn), data(d), len(l), snd(0) {}
  };

  typedef std::deque<Buffer> BufferQueue;
  typedef void (P2Client::*ProtocolHandler)(const Json::Value&);
  typedef std::map<string, ProtocolHandler> ProtocolHandlerMap;

 public:
  P2Client(Poller* poller, const string& peer_ip,
           uint16_t peer_port = DEFAULT_PEER_PORT);

  ~P2Client();

  const char* name() const { return name_.c_str(); }

  int32_t Initialize();

  int32_t Deinitialize();

  void Handle();

 private:
  int32_t InitializeFsm();

  int32_t InitializeHandler();

  ProtocolHandler GetRequestHandler(const string& operation);

  ProtocolHandler GetResponseHandler(const string& operation);

  void startConn();

  void retryConn();

  void keepAlive();

  void handleSendQueue();

  void handleMessage(const Json::Value& value);

  uint32_t msn();

  const char* toString(eLinkState state);

 private:
  virtual void OnRead();

  virtual void OnWrite();

  virtual void OnError();

  void SendMessage(const char* data, uint32_t len);

  void SendMessage(const Json::Value& value);

 private:
  void OnStateDisconn();

  void OnEnterConnect();

  void OnStateConnect();

  void OnEnterVerify();

  void OnStateVerify();

  void OnEnterLogin();

  void OnStateLogin();

  void OnEnterAlive();

  void OnStateAlive();

  void SetState(StateId to);

 private:
  void OnPassengerFlowInfoRequest(const Json::Value& request);

  void OnConnectReply(const Json::Value& reply);

  void OnVerifyReply(const Json::Value& reply);

  void OnLoginReply(const Json::Value& reply);

  void OnGetVersionReply(const Json::Value& reply);

 private:
  string name_;
  Poller* poller_;

  string peer_ip_;
  uint16_t peer_port_;

  bool alived_;
  bool added_;
  int conn_state_;

  FsmState* current_state_;
  FsmState* last_state_;
  FsmState fsm_states_[kLinkTotal];

  BufferQueue sendlist_;
  uint32_t msn_;

  char readbuf_[RECV_BUFLEN];
  ssize_t readpos_;

  ProtocolHandlerMap request_handlers_;
  ProtocolHandlerMap response_handlers_;

  string certificate_s0_;
  string sessionid_;
  bool connect_ok_;
  bool verify_ok_;
  bool login_ok_;
};

}  // namespace app

}  // namespace demo

#endif  // _DEMO_APP_P2_CLIENT_H_
