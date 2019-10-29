#ifndef COMMON_FIREBASE_HELPER_HPP
#define COMMON_FIREBASE_HELPER_HPP

#include <arpa/inet.h>
#include <cstring>
#include <curlpp/Options.hpp>
#include <future>
#include <ifaddrs.h>
#include <sstream>
#include <string>

namespace Common {
namespace FirebaseHelper {
const std::string kBasePath = "https://us-central1-projet3-46f1b.cloudfunctions.net/";
const std::string kDefaultUser = "server";

// Inspired by https://gist.github.com/quietcricket/2521037
inline std::string getSelfIpAddress() {
  std::string ipAddress = "Unable to get IP Address";
  struct ifaddrs* interfaces = nullptr;
  if (getifaddrs(&interfaces) == 0) {
    struct ifaddrs* teamAddr = interfaces;
    while (teamAddr != nullptr) {
      if (teamAddr->ifa_addr->sa_family == AF_INET && (strcmp(teamAddr->ifa_name, "eth0" || strcmp(teamAddr->ifa_name, "wifi0")) == 0) {
        ipAddress = inet_ntoa(((struct sockaddr_in*)teamAddr->ifa_addr)->sin_addr);  // NOLINT
      }
      teamAddr = teamAddr->ifa_next;
    }
  }
  freeifaddrs(interfaces);
  return ipAddress;
}

inline std::string getServerIpAddress(const std::string& user = kDefaultUser) {
  auto resp = curlpp::options::Url(kBasePath + "getServerURL?user=" + user);
  std::stringstream ss;
  ss << resp;
  return ss.str();
}

inline void setIpAddress(const std::string& ipAddress, const std::string& user = kDefaultUser) {
  auto resp = curlpp::options::Url(kBasePath + "setServerURL?user=" + user + "&url=" + ipAddress);
  std::stringstream ss;
  ss << resp;
  if (ss.str() != "OK") {
    std::cout << ss.str() << std::endl;
    std::cout << "Not able to update firebase database" << std::endl;
    exit(1);
  } else {
    std::cout << "Succesfuly able to update firebase database for user " << user << std::endl;
  }
}

inline void setIpAddressAsync(const std::string& ipAddress, const std::string& user = kDefaultUser) {
  std::thread(setIpAddress, ipAddress, user).detach();
}

}  // namespace FirebaseHelper
}  // namespace Common

#endif  // COMMON_FIREBASE_HELPER_HPP
