#ifndef COMMON_SCRIPTS_HELPER_HPP
#define COMMON_SCRIPTS_HELPER_HPP

#include <common/database.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace Common {
namespace ScriptsHelper {

inline void createCert(const std::string& ip) {
  Common::Database db("blockchain.db");
  if (!db.containsIp(ip)) {
    std::cout << "Adding the ip in the database" << std::endl;
    db.addIp(ip.c_str());
    auto ips = db.getIps();

    std::stringstream ss;
    ss << "./createCert.sh server '";
    for (size_t i = 0; i < ips.size(); ++i) {
      ss << "IP." << i + 1 << " = " << ips[i] << std::endl;
    }
    ss << "'";
    std::cout << ss.str() << std::endl;
    system(ss.str().c_str());
  } else {
    std::cout << "Ip already in the database" << std::endl;
  }
}

}  // namespace ScriptsHelper
}  // namespace Common

#endif  // COMMON_SCRIPTS_HELPER_HPP
