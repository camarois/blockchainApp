#ifndef COMMON_FIREBASE_HELPER_HPP
#define COMMON_FIREBASE_HELPER_HPP

#include <arpa/inet.h>
#include <curlpp/Options.hpp>
#include <future>
#include <ifaddrs.h>
#include <string.h>
#include <sstream>
#include <stdio.h>

namespace FirebaseHelper {
const std::string kBasePath = "https://us-central1-projet3-46f1b.cloudfunctions.net/";
const std::string kDefaultUser = "server";

// Inspired by https://gist.github.com/quietcricket/2521037
std::string getSelfIpAddress() {
  std::string ipAddress = "Unable to get IP Address";
  struct ifaddrs* interfaces = NULL;
  if (getifaddrs(&interfaces) == 0) {
    struct ifaddrs* temp_addr = interfaces;
    while (temp_addr != NULL) {
      if (temp_addr->ifa_addr->sa_family == AF_INET && strcmp(temp_addr->ifa_name, "wifi0") == 0) {
	ipAddress = inet_ntoa(((struct sockaddr_in*)temp_addr->ifa_addr)->sin_addr);
      }
      temp_addr = temp_addr->ifa_next;
    }
  }
  freeifaddrs(interfaces);
  return ipAddress;
}

std::string getServerIpAddress(const std::string& user = kDefaultUser) {
  std::ostringstream oss;
  oss << kBasePath << "getServerURL?user=" << user;
  auto resp = curlpp::options::Url(oss.str());
  oss.str("");
  oss << resp;
  return oss.str();
}

void setIpAddress(const std::string& ipAddress, const std::string& user = kDefaultUser) {
  std::ostringstream oss;
  oss << kBasePath << "setServerURL?user=" << user << "&url=" << ipAddress;
  auto resp = curlpp::options::Url(oss.str());
  oss.str("");
  oss << resp;
  if (oss.str() != "OK") {
    std::cout << oss.str() << std::endl;
    std::cout << "Not able to update firebase database" << std::endl;
    exit(1);
  } else {
    std::cout << "Succesfuly able to update firebase database for user " << user << std::endl;
  }
}

std::future<void> setIpAddressAsync(const std::string& ipAddress,
				    const std::string& user = kDefaultUser) {
  return std::async(std::launch::async, setIpAddress, ipAddress, user);
}

}  // namespace FirebaseHelper

#endif  // COMMON_FIREBASE_HELPER_HPP
