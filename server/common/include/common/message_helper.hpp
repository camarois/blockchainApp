#ifndef COMMON_MESSAGE_HELPER_HPP
#define COMMON_MESSAGE_HELPER_HPP

#include <common/models.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <zmq.hpp>

namespace Common {
namespace MessageHelper {

inline std::string toString(const zmq::message_t& message) {
  return std::string(static_cast<const char*>(message.data()), message.size());
}

inline zmq::message_t fromString(const std::string& str) {
  zmq::message_t mnessage(str.length());
  memcpy(mnessage.data(), str.data(), str.length());
  return mnessage;
}

inline nlohmann::json toJSON(const zmq::message_t& message) {
  return nlohmann::json::parse(toString(message));
}

inline zmq::message_t fromJSON(const nlohmann::json& json) {
  return fromString(json.dump());
}

template <class T>
inline T toModel(const zmq::message_t& message) {
  return Models::fromStr<T>(toString(message));
}

template <class T>
inline zmq::message_t fromModel(const T& model) {
  return fromString(Models::toStr(model));
}

}  // namespace MessageHelper
}  // namespace Common

#endif  // COMMON_MESSAGE_HELPER_HPP
