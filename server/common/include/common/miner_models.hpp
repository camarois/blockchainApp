#ifndef COMMON_MINER_MODELS_HPP
#define COMMON_MINER_MODELS_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace Common {
namespace Models {

const std::string kBlockID = "block-id";
const std::string kBlockNonce = "block-nonce";
const std::string kCommand = "command";
const std::string kContent = "content";
const std::string kData = "data";
const std::string kID = "id";
const std::string kResult = "result";
const std::string kToken = "token";
const std::string kType = "type";
const std::string kTypeBlockMined = "block-mined";
const std::string kTypeMinerReady = "miner-ready";
const std::string kTypeServerRequest = "server-request";
const std::string kTypeServerResponse = "server-response";
const std::string kTypeTransaction = "transaction";

struct ZMQMessage {
  std::string type;
  std::string data;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const ZMQMessage& obj) {
  j = {{kType, obj.type}, {kData, obj.data}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, ZMQMessage& obj) {
  j.at(kType).get_to(obj.type);
  j.at(kData).get_to(obj.data);
}

struct MinerReady {
  // TODO(gabriel): need anything?
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const MinerReady& obj) {
  // TODO(gabriel): need anything?
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, MinerReady& obj) {
  // TODO(gabriel): need anything?
}

struct NewTransaction {
  std::string content;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const NewTransaction& obj) {
  j = {{kContent, obj.content}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, NewTransaction& obj) {
  j.at(kContent).get_to(obj.content);
}

struct BlockMined {
  unsigned int id;
  unsigned int nonce;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const BlockMined& obj) {
  j = {{kBlockID, obj.id}, {kBlockNonce, obj.nonce}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, BlockMined& obj) {
  j.at(kBlockID).get_to(obj.id);
  j.at(kBlockNonce).get_to(obj.nonce);
}

struct ServerRequest {
  std::string id;
  std::string token;
  std::string command;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const ServerRequest& obj) {
  j = {{kID, obj.id}, {kToken, obj.token}, {kCommand, obj.command}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, ServerRequest& obj) {
  j.at(kID).get_to(obj.id);
  j.at(kToken).get_to(obj.token);
  j.at(kCommand).get_to(obj.command);
}

struct ServerResponse {
  std::string id;
  std::string token;
  std::string result;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const ServerResponse& obj) {
  j = {{kID, obj.id}, {kToken, obj.token}, {kResult, obj.result}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, ServerResponse& obj) {
  j.at(kID).get_to(obj.id);
  j.at(kToken).get_to(obj.token);
  j.at(kResult).get_to(obj.result);
}

}  // namespace Models
}  // namespace Common

#endif  // COMMON_MINER_MODELS_HPP
