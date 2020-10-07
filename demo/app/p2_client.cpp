#include "app/p2_client.h"
#include "base/common_utils.h"
#include "base/log.h"
#include "base/md5.h"
#include "net/n9m_protocol.h"
#include "net/socket_utils.h"

using namespace demo::base;
using namespace demo::net;

#include <assert.h>
#include <errno.h>
#include <fcntl.h>

namespace demo {

namespace app {

P2Client::P2Client(Poller* poller, const string& peer_ip, uint16_t peer_port)
    : peer_ip_(peer_ip),
      peer_port_(peer_port),
      alived_(false),
      added_(false),
      conn_state_(kDisconnected),
      current_state_(NULL),
      last_state_(NULL),
      msn_(0),
      poller_(poller),
      readpos_(0),
      connect_ok_(false),
      verify_ok_(false),
      login_ok_(false) {
  name_.append("P2_");
  name_.append(peer_ip);

  for (int i = 0; i < kLinkTotal; ++i) {
    fsm_states_[i].id = static_cast<StateId>(i);
    fsm_states_[i].name = "";
    fsm_states_[i].onState = NULL;
    fsm_states_[i].onEnter = NULL;
    fsm_states_[i].onExit = NULL;
  }

  current_state_ = &fsm_states_[kLinkDisconn];
}

P2Client::~P2Client() {
  if (added_) {
    poller_->DelChannel(this);
  }

  Deinitialize();
}

int32_t P2Client::Initialize() {
  set_events(E_READ | E_WRITE);

  if (0 != InitializeFsm()) {
    return -1;
  }

  if (0 != InitializeHandler()) {
    return -2;
  }

  return 0;
}

int32_t P2Client::InitializeFsm() {
  fsm_states_[kLinkDisconn].onState = &P2Client::OnStateDisconn;

  fsm_states_[kLinkConnect].onState = &P2Client::OnStateConnect;
  fsm_states_[kLinkConnect].onEnter = &P2Client::OnEnterConnect;

  fsm_states_[kLinkVerify].onState = &P2Client::OnStateVerify;
  fsm_states_[kLinkVerify].onEnter = &P2Client::OnEnterVerify;

  fsm_states_[kLinkLogin].onState = &P2Client::OnStateLogin;
  fsm_states_[kLinkLogin].onEnter = &P2Client::OnEnterLogin;

  fsm_states_[kLinkAlive].onState = &P2Client::OnStateAlive;
  fsm_states_[kLinkAlive].onEnter = &P2Client::OnEnterAlive;

  return 0;
}

int32_t P2Client::InitializeHandler() {
  // protocol request handlers
  request_handlers_["UPSUPPLEDEVINFO"] = &P2Client::OnPassengerFlowInfoRequest;

  // protocol response handlers
  response_handlers_["CONNECT"] = &P2Client::OnConnectReply;
  response_handlers_["VERIFY"] = &P2Client::OnVerifyReply;
  response_handlers_["LOGIN"] = &P2Client::OnLoginReply;
  response_handlers_["GETDEVVERSIONINFO"] = &P2Client::OnGetVersionReply;

  return 0;
}

P2Client::ProtocolHandler P2Client::GetRequestHandler(const string& operation) {
  if (request_handlers_.find(operation) != request_handlers_.end()) {
    return request_handlers_[operation];
  }
  return NULL;
}

P2Client::ProtocolHandler P2Client::GetResponseHandler(
    const string& operation) {
  if (response_handlers_.find(operation) != response_handlers_.end()) {
    return response_handlers_[operation];
  }
  return NULL;
}

int32_t P2Client::Deinitialize() {
  if (fd_ >= 0) {
    assert(::close(fd_) >= 0);
  }

  return 0;
}

void P2Client::Handle() {
  FsmHandler handler = current_state_->onState;

  if (handler) {
    (this->*handler)();
  }
}

void P2Client::OnRead() {
  ssize_t nread =
      SocketUtils::Read(fd_, readbuf_ + readpos_, RECV_BUFLEN - readpos_);

  if (nread < 0 || nread > (RECV_BUFLEN - readpos_)) {
    LOG_ERROR("%s read error: nread[%d]err[%s]", name(), nread,
              strerror(errno));
    OnError();
    return;
  }

  if (nread == 0) {
    return;
  }

  LOG_TRACE("%s read %d bytes data: readpos[%d].", name(), nread, readpos_);

  readpos_ += nread;

  if (readpos_ >= sizeof(N9mProtocol)) {
    N9mProtocol* head = (N9mProtocol*)(readbuf_);
    uint32_t counts = ntohl(head->counts);
    uint32_t csrclen = head->CSRC * 4;
    uint32_t packsize = sizeof(N9mProtocol) + csrclen + counts;

    if (readpos_ >= packsize) {
      const char* payload = readbuf_ + sizeof(N9mProtocol) + csrclen;

      if (PT_SIGNAL == head->PT) {
        Json::Value value;
        if (JsonUtils::ParseJsonContext(payload, counts, value)) {
          handleMessage(value);
        }
      }

      memcpy(readbuf_, readbuf_ + packsize, readpos_ - packsize);
      readpos_ = 0;
    }
  }
}

void P2Client::OnWrite() {
  if (kConnecting == conn_state_) {
    if (!SocketUtils::GetSocketErr(fd_)) {
      conn_state_ = kConnected;
      LOG_INFO("%s sockfd %d connected.", name(), fd_);
    } else {
      LOG_ERROR("%s sockfd %d error %d.", name(), fd_,
                SocketUtils::GetSocketErr(fd_));
    }
  } else if (kConnected == conn_state_) {
    handleSendQueue();
  } else {
  }
}

void P2Client::OnError() {
  SetState(kLinkDisconn);
  retryConn();
}

void P2Client::SendMessage(const char* data, uint32_t len) {
  char* buf = new (std::nothrow) char[len + sizeof(N9mProtocol)];

  if (!buf) {
    LOG_ERROR("%s bad alloc.", name());
    return;
  }

  N9mProtocol* head = (N9mProtocol*)buf;
  memset(head, 0, sizeof(N9mProtocol));
  head->M = 1;
  head->CSRC = 0;
  head->PT = PT_SIGNAL;
  head->SSRC = 0;
  head->counts = htonl(len);
  head->reserve = 'R';
  memcpy(buf + sizeof(N9mProtocol), data, len);

  // CommonUtils::Hexdump(buf, len + sizeof(N9mProtocol));

  uint32_t sn = msn();
  sendlist_.push_back(Buffer(sn, buf, len + sizeof(N9mProtocol)));
  LOG_TRACE("%s sndsize %d add message [sn:%u len:%u]%s", name(),
            sendlist_.size(), sn, len, data);
}

void P2Client::SendMessage(const Json::Value& value) {
  string value_str = JsonUtils::ToString(value);
  SendMessage(value_str.c_str(), value_str.size());
}

void P2Client::startConn() {
  if (fd_ < 0) {
    fd_ = SocketUtils::CreateNonBlockOrDie(AF_INET);
    LOG_INFO("%s create socket %d success.", name(), fd_);

    const char* dev = "eth0";
    if (SocketUtils::BindToDevice(fd_, dev)) {
      LOG_INFO("%s bind sockfd %d to dev %s success.", name(), fd_, dev);
    } else {
      LOG_WARN("%s bind sockfd %d to dev %s failed.", name(), fd_, dev);
    }
  }

  LOG_INFO("%s attempt to connect %s:%d", name(), peer_ip_.c_str(), peer_port_);

  struct sockaddr_in peer_addr;
  bzero(&peer_addr, sizeof(struct sockaddr_in));

  peer_addr.sin_family = AF_INET;
  peer_addr.sin_port = htons(peer_port_);
  peer_addr.sin_addr.s_addr = inet_addr(peer_ip_.c_str());

  int ret = SocketUtils::Connect(fd_, (const struct sockaddr*)&peer_addr);
  int saved_errno = (ret == 0) ? 0 : errno;

  switch (saved_errno) {
    case 0:
    case EINPROGRESS:
    case EINTR:
    case EISCONN: {
      // connecting
      conn_state_ = kConnecting;
      poller_->AddChannel(this);
      added_ = true;
    } break;

    case EAGAIN:
    case EADDRINUSE:
    case EADDRNOTAVAIL:
    case ECONNREFUSED:
    case ENETUNREACH: {
      // retry
      retryConn();
    } break;

    default:
      LOG_FATAL("connect error: %d, %s", saved_errno, strerror(saved_errno));
      break;
  }
}

void P2Client::retryConn() {
  SocketUtils::Close(fd_);
  conn_state_ = kDisconnected;
  fd_ = -1;
}

void P2Client::keepAlive() {
  static time_t last_keepalive = 0;
  if ((time(0) - last_keepalive) >= 10) {
    last_keepalive = time(0);

    Json::Value request;
    request["MODULE"] = "CERTIFICATE";
    request["SESSION"] = sessionid_;
    request["OPERATION"] = "KEEPALIVE";
    SendMessage(request);
  }
}

void P2Client::handleSendQueue() {
  if (!sendlist_.empty()) {
    Buffer buffer = sendlist_.front();

    ssize_t nwrote = SocketUtils::Write(fd(), buffer.data + buffer.snd,
                                        buffer.len - buffer.snd);

    if (nwrote > 0) {
      buffer.snd += nwrote;
    } else {
      if (errno != EWOULDBLOCK) {
        LOG_ERROR("%s write error: %d, %s", name(), errno, strerror(errno));
        OnError();
      }
    }

    if (buffer.snd >= buffer.len) {
      LOG_TRACE("%s message [sn:%u len:%u]%s send complete.", name(),
                buffer.msn, buffer.len, buffer.data);
      delete[] buffer.data;
      sendlist_.pop_front();
    }

    LOG_TRACE("%s send queue size: %d.", name(), sendlist_.size());
  }
}

void P2Client::handleMessage(const Json::Value& value) {
  LOG_INFO("%s handle message: %s", name(), JsonUtils::ToString(value).c_str());

  string operation = JsonUtils::AsString(value, "OPERATION");

  if (operation.empty()) {
    LOG_WARN("%s invalid message: %s", name(),
             JsonUtils::ToString(value).c_str());
  }

  if (value.isMember("PARAMETER")) {
    ProtocolHandler handler = GetRequestHandler(operation);
    if (handler) {
      (this->*handler)(value);
    }
  } else if (value.isMember("RESPONSE")) {
    ProtocolHandler handler = GetResponseHandler(operation);
    if (handler) {
      (this->*handler)(value);
    }
  } else {
    // TODO:
  }
}

uint32_t P2Client::msn() { return msn_++; }

const char* P2Client::toString(eLinkState state) {
  switch (state) {
    case kLinkDisconn:
      return "Disconnted";
    case kLinkConnect:
      return "Connecting";
    case kLinkVerify:
      return "Verifying";
    case kLinkLogin:
      return "Logining";
    case kLinkAlive:
      return "Alive";
    default:
      return "Unknown State";
  }
}

void P2Client::OnStateDisconn() {
  static time_t last_conn_try = 0;

  if (kDisconnected == conn_state_ && (time(0) - last_conn_try) >= 5) {
    last_conn_try = time(0);
    startConn();
  }

  if (kConnected == conn_state_) {
    SetState(kLinkConnect);
  }
}

void P2Client::OnEnterConnect() {
  Json::Value request;
  request["MODULE"] = "CERTIFICATE";
  request["OPERATION"] = "CONNECT";
  request["PARAMETER"]["MODE"] = 0;

  SendMessage(request);

  connect_ok_ = false;
}

void P2Client::OnStateConnect() {
  if (connect_ok_) {
    SetState(kLinkVerify);
  }
}

void P2Client::OnEnterVerify() {
  Json::Value request;
  request["MODULE"] = "CERTIFICATE";
  request["SESSION"] = sessionid_;
  request["OPERATION"] = "VERIFY";

  char buf[64] = "";
  strncpy(buf, certificate_s0_.c_str(), sizeof(buf));

  request["PARAMETER"]["S0"] = base::hmac_md5(buf, buf);

  SendMessage(request);

  verify_ok_ = false;
}

void P2Client::OnStateVerify() {
  if (verify_ok_) {
    SetState(kLinkLogin);
  }
}

void P2Client::OnEnterLogin() {
  // TODO:
  const char* key = "streaming";
  const char* username = "admin";
  const char* password = "";

  Json::Value request;
  request["MODULE"] = "CERTIFICATE";
  request["SESSION"] = sessionid_;
  request["OPERATION"] = "LOGIN";
  request["PARAMETER"]["USER"] = username;
  request["PARAMETER"]["PASSWD"] = base::hmac_md5((char*)password, (char*)key);
  request["PARAMETER"]["LOGINTYPE"] = 1;

  SendMessage(request);

  login_ok_ = false;
}

void P2Client::OnStateLogin() {
  if (login_ok_) {
    SetState(kLinkAlive);
  }
}

void P2Client::OnEnterAlive() {
  // Get P2 version info
  Json::Value request;
  request["MODULE"] = "DEVEMM";
  request["SESSION"] = sessionid_;
  request["OPERATION"] = "GETDEVVERSIONINFO";

  SendMessage(request);

  // TODO: set utc
  // TODO: set parameter
  // TODO: ...
}

void P2Client::OnStateAlive() {
  keepAlive();

  // TODO:
  // set door state
}

void P2Client::SetState(StateId to) {
  if (to < 0 || to >= kLinkTotal) {
    return;
  }

  if (current_state_->id == to) {
    return;
  }

  FsmState* prev = current_state_;
  FsmState* next = &fsm_states_[to];

  LOG_INFO("%s set state [%s] to [%s].", name(), toString(prev->id),
           toString(next->id));

  FsmHandler onExit = prev->onExit;
  if (onExit) {
    (this->*onExit)();
  }

  last_state_ = prev;
  current_state_ = next;

  FsmHandler onEnter = next->onEnter;
  if (onEnter) {
    (this->*onEnter)();
  }
}

void P2Client::OnPassengerFlowInfoRequest(const Json::Value& request) {}

void P2Client::OnConnectReply(const Json::Value& reply) {
  sessionid_ = JsonUtils::AsString(reply, "SESSION");
  certificate_s0_ = JsonUtils::AsString(reply["RESPONSE"], "S0");

  connect_ok_ = true;
}

void P2Client::OnVerifyReply(const Json::Value& reply) {
  bool ret = JsonUtils::AsBool(reply["RESPONSE"], "RETURN");

  if (ret) {
    verify_ok_ = true;
  }
}

void P2Client::OnLoginReply(const Json::Value& reply) {
  bool ret = JsonUtils::AsBool(reply["RESPONSE"], "RETURN");

  if (ret) {
    login_ok_ = true;
  }
}

void P2Client::OnGetVersionReply(const Json::Value& reply) {
  int errcode = JsonUtils::AsInt(reply["RESPONSE"], "ERRORCODE");

  if (0 == errcode) {
    const char* dev_number = JsonUtils::AsCString(reply["RESPONSE"], "DSNO");
    const char* dev_mainver =
        JsonUtils::AsCString(reply["RESPONSE"]["DEVINFO"], "MANVERSION");
    const char* tofmcu_ver =
        JsonUtils::AsCString(reply["RESPONSE"]["DEVINFO"], "TOFV");

    LOG_INFO("%s Device Number      : %s", name(), dev_number);
    LOG_INFO("%s Main Version       : %s", name(), dev_mainver);
    LOG_INFO("%s TOF Mcu Version    : %s", name(), tofmcu_ver);
  } else {
    LOG_WARN("%s get version failed.", name());
  }
}

}  // namespace app

}  // namespace demo
