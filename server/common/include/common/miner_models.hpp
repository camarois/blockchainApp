#ifndef COMMON_MINER_MODELS_HPP
#define COMMON_MINER_MODELS_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace Common {

enum Functions {
  Unknown,
  AddUser,
  DeleteUser,
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
  GetAllUsers,
  GetLogs
};

namespace Models {

const std::string kBlocks = "blocks";
const std::string kBlockID = "block-id";
const std::string kLastID = "last-id";
const std::string kBlockNonce = "block-nonce";
const std::string kBlockCount = "block-count";
const std::string kCommand = "command";
const std::string kContent = "content";
const std::string kFound = "found";
const std::string kData = "data";
const std::string kID = "id";
const std::string kHash = "hash";
const std::string kNonce = "nonce";
const std::string kResult = "result";
const std::string kToken = "token";
const std::string kType = "type";
const std::string kFunction = "function";
const std::string kParams = "params";
const std::string kMinerID = "miner-id";
const std::string kSelfId = "self-id";
const std::string kNumberOfVerifications = "number-of-verifications";
const std::string kTypeBlockSyncRequest = "block-sync-request";
const std::string kTypeBlockSyncResponse = "block-sync-response";
const std::string kTypeMinerId = "miner-id";
const std::string kTypeBlockMined = "block-mined";
const std::string kTypeMinerReady = "miner-ready";
const std::string kTypeServerRequest = "get-request";
const std::string kTypeServerResponse = "get-response";
const std::string kTypeTransaction = "update-transaction";
const std::string kTypeGetBlocksRequest = "get-blocks-request";
const std::string kTypeGetBlocksResponse = "get-blocks-response";
const std::string kTypeLogRequest = "get-log-request";

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
  int lastId = -1;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const BlockSyncRequest& obj) { j = {{kLastID, obj.lastId}}; }

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, BlockSyncRequest& obj) { j.at(kLastID).get_to(obj.lastId); }

struct BlockMined {
  int id = -1;
  int nonce = 0;
  int numberOfVerifications = 0;
  std::string data;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const BlockMined& obj) {
  j = {{kBlockID, obj.id},
       {kBlockNonce, obj.nonce},
       {kNumberOfVerifications, obj.numberOfVerifications},
       {kData, obj.data}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, BlockMined& obj) {
  j.at(kBlockID).get_to(obj.id);
  j.at(kBlockNonce).get_to(obj.nonce);
  j.at(kNumberOfVerifications).get_to(obj.numberOfVerifications);
  j.at(kData).get_to(obj.data);
}

struct BlockSyncResponse {
  std::vector<BlockMined> blocks;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const BlockSyncResponse& obj) { j = {{kBlocks, obj.blocks}}; }

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, BlockSyncResponse& obj) { j.at(kBlocks).get_to(obj.blocks); }

struct ServerRequest {
  std::string token;
  std::string command;
  int lastBlockId = -1;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const ServerRequest& obj) {
  j = {{kToken, obj.token}, {kCommand, obj.command}, {kLastID, obj.lastBlockId}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, ServerRequest& obj) {
  j.at(kToken).get_to(obj.token);
  j.at(kCommand).get_to(obj.command);
  j.at(kLastID).get_to(obj.lastBlockId);
}

struct ServerResponse {
  std::string token;
  std::string result;
  int lastBlockId = -1;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const ServerResponse& obj) {
  j = {{kToken, obj.token}, {kResult, obj.result}, {kLastID, obj.lastBlockId}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, ServerResponse& obj) {
  j.at(kToken).get_to(obj.token);
  j.at(kResult).get_to(obj.result);
  j.at(kLastID).get_to(obj.lastBlockId);
}

struct ReadyResponse {
  std::string token;
  int selfId = -1;
  int lastBlockId = -1;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const ReadyResponse& obj) {
  j = {{kToken, obj.token}, {kSelfId, obj.selfId}, {kLastID, obj.lastBlockId}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, ReadyResponse& obj) {
  j.at(kToken).get_to(obj.token);
  j.at(kSelfId).get_to(obj.selfId);
  j.at(kLastID).get_to(obj.lastBlockId);
}

struct GetBlocksRequest {
  // std::string token = "0";
  int blockCount = 0;
  int minerID = 0;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const GetBlocksRequest& obj) {
  j = {/*{kToken, obj.token},*/ {kBlockCount, obj.blockCount}, {kMinerID, obj.minerID}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, GetBlocksRequest& obj) {
  /*j.at(kToken).get_to(obj.token);*/
  j.at(kBlockCount).get_to(obj.blockCount);
  j.at(kMinerID).get_to(obj.minerID);
}

struct Block {
  int id = -1;
  int nonce = -1;
  std::string hash;
  std::string content;
  int numberOfVerifications = 0;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const Block& obj) {
  j = {{kID, obj.id}, {kNonce, obj.nonce}, {kHash, obj.hash}, {kContent, obj.content}, {kNumberOfVerifications, obj.numberOfVerifications}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, Block& obj) {
  j.at(kID).get_to(obj.id);
  j.at(kNonce).get_to(obj.nonce);
  j.at(kHash).get_to(obj.hash);
  j.at(kContent).get_to(obj.content);
  j.at(kNumberOfVerifications).get_to(obj.numberOfVerifications);
}

struct GetBlocksResponse {
  std::vector<std::string> blocks;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const GetBlocksResponse& obj) {
  j = {
      {kBlocks, obj.blocks},
  };
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, GetBlocksResponse& obj) { j.at(kBlocks).get_to(obj.blocks); }

// struct ChainRequest
// {
//   int blockCount = 0;
// };

// // NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
// inline void to_json(nlohmann::json& j, const ChainRequest& obj) {
//   j = {
//       {kBlockCount, obj.blockCount},
//   };
// }

// // NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
// inline void from_json(const nlohmann::json& j, ChainRequest& obj) { j.at(kBlockCount).get_to(obj.blockCount); }



}  // namespace Models
}  // namespace Common

#endif  // COMMON_MINER_MODELS_HPP
