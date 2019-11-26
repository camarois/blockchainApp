#include "common/firebase_helper.hpp"
#include "common/gflags_helper.hpp"
#include "common/logger.hpp"
#include "common/message_helper.hpp"
#include <common/database.hpp>
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
DEFINE_int32(difficulty, 3, "Hashing difficulty");                      // NOLINT

int main(int argc, char* argv[]) {
  Common::GflagsHelper::init("Blockchain miner service", argc, argv);
  Common::Database::init(FLAGS_db);

  std::string addr = FLAGS_addr;
  if (addr.empty()) {
    addr = "tcp://" + Common::FirebaseHelper::getServerIpAddress(FLAGS_user);
  }
  Common::Logger::get()->info("Server ip address: " + addr + "\n");

  Miner::ZMQWorker miner(addr);
  miner.start();
  miner.join();

  return 0;
}
