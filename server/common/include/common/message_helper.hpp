#ifndef COMMON_MESSAGE_HELPER_HPP
#define COMMON_MESSAGE_HELPER_HPP
#include <string>
#include <zmq.hpp>

namespace MessageHelper {
inline std::string toString(const zmq::message_t& message) {
  return std::string(static_cast<const char*>(message.data()), message.size());
}

inline zmq::message_t fromString(const std::string& str) {
  zmq::message_t mnessage(str.length());
  memcpy(mnessage.data(), str.data(), str.length());
  return mnessage;
}
}  // namespace MessageHelper

#endif // COMMON_MESSAGE_HELPER_HPP
