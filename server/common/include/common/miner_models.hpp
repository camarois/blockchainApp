#ifndef COMMON_MINER_MODELS_HPP
#define COMMON_MINER_MODELS_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace Common {

enum Functions {
  Unknown,
  AddUser,
  SetUserPassword,
  ContainsUser,
  ContainsAdmin,
  GetRole,
  GetSalt,
  CheckForExistingClass,
  DeleteExistingClass,
  DeleteExistingResults,
  AddNewClass,
  AddNewResult,
  GetClassResult,
  GetStudentResult,
  GetClasses,
  GetStudents,
  GetLogs
};

namespace Models {

const std::string kBlocks = "blocks";
const std::string kBlockID = "block-id";
const std::string kLastID = "last-id";
const std::string kBlockNonce = "block-nonce";
const std::string kCommand = "command";
const std::string kContent = "content";
const std::string kFound = "found";
const std::string kData = "data";
const std::string kID = "id";
const std::string kResult = "result";
const std::string kToken = "token";
const std::string kType = "type";
const std::string kFunction = "function";
const std::string kParams = "params";

const std::string kTypeBlockSyncRequest = "block-sync-request";
const std::string kTypeBlockSyncResponse = "block-sync-response";
const std::string kTypeMinerId = "miner-id";
const std::string kTypeBlockMined = "block-mined";
const std::string kTypeMinerReady = "miner-ready";
const std::string kTypeServerRequest = "get-request";
const std::string kTypeServerResponse = "get-response";
const std::string kTypeTransaction = "update-transaction";

struct SqlRequest {
  Functions function = Functions::Unknown;
  std::string params;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const SqlRequest& obj) {
  j = {{kFunction, obj.function}, {kParams, obj.params}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, SqlRequest& obj) {
  j.at(kFunction).get_to(obj.function);
  j.at(kParams).get_to(obj.params);
}

struct SqlResponse {
  bool found = false;
  std::string data;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const SqlResponse& obj) { j = {{kFound, obj.found}, {kData, obj.data}}; }

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, SqlResponse& obj) {
  j.at(kFound).get_to(obj.found);
  j.at(kData).get_to(obj.data);
}

struct ZMQMessage {
  std::string type;
  std::string data;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const ZMQMessage& obj) { j = {{kType, obj.type}, {kData, obj.data}}; }

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, ZMQMessage& obj) {
  j.at(kType).get_to(obj.type);
  j.at(kData).get_to(obj.data);
}

struct BlockSyncRequest {
  unsigned int lastId = 0;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const BlockSyncRequest& obj) { j = {{kLastID, obj.lastId}}; }

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, BlockSyncRequest& obj) {
  j.at(kLastID).get_to(obj.lastId);
}

struct BlockMined {
  unsigned int id = 0;
  unsigned int nonce = 0;
  std::string data;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const BlockMined& obj) { j = {{kBlockID, obj.id}, {kBlockNonce, obj.nonce}, {kData, obj.data}}; }

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, BlockMined& obj) {
  j.at(kBlockID).get_to(obj.id);
  j.at(kBlockNonce).get_to(obj.nonce);
  j.at(kData).get_to(obj.data);
}

struct BlockSyncResponse {
  std::vector<BlockMined> blocks;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const BlockSyncResponse& obj) { j = {{kBlocks, obj.blocks}}; }

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, BlockSyncResponse& obj) {
  j.at(kBlocks).get_to(obj.blocks);
}

struct ServerRequest {
  std::string token;
  std::string command;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const ServerRequest& obj) { j = {{kToken, obj.token}, {kCommand, obj.command}}; }

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, ServerRequest& obj) {
  j.at(kToken).get_to(obj.token);
  j.at(kCommand).get_to(obj.command);
}

struct ServerResponse {
  std::string token;
  std::string result;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const ServerResponse& obj) { j = {{kToken, obj.token}, {kResult, obj.result}}; }

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, ServerResponse& obj) {
  j.at(kToken).get_to(obj.token);
  j.at(kResult).get_to(obj.result);
}

}  // namespace Models
}  // namespace Common

#endif  // COMMON_MINER_MODELS_HPP
