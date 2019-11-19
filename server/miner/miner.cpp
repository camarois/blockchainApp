#include "common/firebase_helper.hpp"
#include "common/gflags_helper.hpp"
#include "common/logger.hpp"
#include "common/message_helper.hpp"
#include <iostream>
#include <memory>
#include <miner/blockchain.hpp>
#include <miner/zmq.hpp>
#include <string>
#include <unistd.h>
#include <zmq.hpp>

DEFINE_string(addr, "", "REST service address");                        // NOLINT
DEFINE_string(user, "server", "Developper using the service");          // NOLINT
DEFINE_string(db, "blockchain.db", "Path to sqlite db file");           // NOLINT
DEFINE_string(blockchain, "blockchain/", "Path to blockchain folder");  // NOLINT
DEFINE_int32(id, -1, "Miner id");                                       // NOLINT
DEFINE_int32(difficulty, 3, "Hashing difficulty");                      // NOLINT

int main(int argc, char* argv[]) {
  Common::GflagsHelper::init("Blockchain miner service", argc, argv);
  Common::Logger::init(FLAGS_db);

  std::string addr = FLAGS_addr;
  if (addr.empty()) {
    addr = "tcp://" + Common::FirebaseHelper::getServerIpAddress(FLAGS_user);
  }
  Common::Logger::get()->info(FLAGS_id, "Server ip address: " + addr + "\n");

  std::optional<Miner::BlockChain> maybeBlockchain =
      Miner::BlockChain::fromDirectory(std::filesystem::path(FLAGS_blockchain));
  std::unique_ptr<Miner::BlockChain> blockchain = std::make_unique<Miner::BlockChain>(maybeBlockchain.value());

  Miner::ZMQWorker miner(addr, std::move(blockchain));
  miner.start();
  miner.join();

  return 0;
}
