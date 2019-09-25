#include <string>
#include <zmq.hpp>

namespace MessageHelper {
inline std::string to_string(const zmq::message_t& message) {
  return std::string(static_cast<const char*>(message.data()), message.size());
}

inline zmq::message_t from_string(const std::string& str) {
  zmq::message_t mnessage(str.length());
  memcpy(mnessage.data(), str.data(), str.length());
  return mnessage;
}
}  // namespace MessageHelper
