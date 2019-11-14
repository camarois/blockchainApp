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

inline std::optional<std::filesystem::path> getSharedFile(const std::string& file) {
  std::filesystem::path local = std::filesystem::current_path() / file;
  if (std::filesystem::exists(local)) {
    return local;
  }

  std::filesystem::path system = std::filesystem::path("/usr/share") / file;
  if (std::filesystem::exists(system)) {
    return system;
  }

  return {};
}

inline bool createCert(const std::string& ip) {
  if (!Common::Database::get()->containsIp(ip)) {
    std::cout << "Adding the ip in the database" << std::endl;
    Common::Database::get()->addIp(ip);
    auto ips = Common::Database::get()->getIps();

    std::optional<std::filesystem::path> script = getSharedFile("createCert.sh");
    if (!script) {
      std::cerr << "Could not find createCert.sh" << std::endl;
      return false;
    }

    std::optional<std::filesystem::path> script = getSharedFile("createCert.sh");
    if (!script) {
      std::cerr << "Could not find createCert.sh" << std::endl;
      return false;
    }

    std::stringstream ss;
    ss << script->string() + " server '";
    for (size_t i = 0; i < ips.size(); ++i) {
      ss << "IP." << i + 1 << " = " << ips.at(i) << std::endl;
    }
    ss << "'";
    int exitCode = std::system(ss.str().c_str());  // NOLINT(cert-env33-c)
    if (exitCode == 0) {
      std::cout << "Ip succesfully added" << std::endl;
    } else {
      std::cout << "createCert.sh exited with exit code: " << exitCode << std::endl;
      return false;
    }
  }

  return true;
}

inline bool createDb(const std::string& dbPath) {
  if (!std::filesystem::exists(dbPath)) {
    std::cout << "Creating the database at " << dbPath << std::endl;
    std::optional<std::filesystem::path> script = getSharedFile("createDb.sh");
    if (!script) {
      std::cerr << "Could not find createDb.sh" << std::endl;
      return false;
    }

    int exitCode = std::system((script->string() + " " + dbPath).c_str());  // NOLINT(cert-env33-c)
    if (exitCode == 0) {
      std::cout << "Database created succesfully" << std::endl;
    } else {
      std::cout << "createDb.sh exited with exit code: " << exitCode << std::endl;
      return false;
    }
  }

  return true;
}

}  // namespace ScriptsHelper
}  // namespace Common

#endif  // COMMON_SCRIPTS_HELPER_HPP
