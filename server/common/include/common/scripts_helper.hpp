#ifndef COMMON_SCRIPTS_HELPER_HPP
#define COMMON_SCRIPTS_HELPER_HPP

#include <common/database.hpp>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace Common {
namespace ScriptsHelper {

inline void createCert(const std::string& ip, const std::string& dbPath) {
  if (!Common::Database::get()->containsIp(ip)) {
    std::cout << "Adding the ip in the database" << std::endl;
    Common::Database::get()->addIp(ip);
    auto ips = Common::Database::get()->getIps();

    std::stringstream ss;
    ss << "./createCert.sh server '";
    for (size_t i = 0; i < ips.size(); ++i) {
      ss << "IP." << i + 1 << " = " << ips.at(i) << std::endl;
    }
    ss << "'";
    int exitCode = std::system(ss.str().c_str());  // NOLINT(cert-env33-c)
    if (exitCode == 0) {
      std::cout << "Ip succesfully added" << std::endl;
    } else {
      std::cout << "createCert.sh exited with exit code: " << exitCode << std::endl;
    }
  }
}

inline void createDb(const std::string& dbPath) {
  if (!std::filesystem::exists(dbPath)) {
    std::cout << "Creating the database at " << dbPath << std::endl;

    int exitCode = std::system(("./createDb.sh " + dbPath).c_str());  // NOLINT(cert-env33-c)
    if (exitCode == 0) {
      std::cout << "Database created succesfully" << std::endl;
    } else {
      std::cout << "createDb.sh exited with exit code: " << exitCode << std::endl;
    }
  }
}

}  // namespace ScriptsHelper
}  // namespace Common

#endif  // COMMON_SCRIPTS_HELPER_HPP
