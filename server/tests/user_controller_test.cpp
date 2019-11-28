#include "rest_utils.hpp"
#include <chrono>
#include <common/models.hpp>
#include <cstdlib>
#include <exception>
#include <experimental/filesystem>
#include <filesystem>
#include <fstream>
#include <future>
#include <gflags/gflags.h>
#include <gtest/gtest.h>
#include <iostream>
#include <rest/custom_router.hpp>
#include <rest/main_controller.hpp>
#include <rest/user_controller.hpp>
#include <rest/zmq.hpp>
#include <thread>

DECLARE_int32(FLAGS_port);

TEST(UserControllerTest, start) {
  std::shared_ptr<Rest::CustomRouter> router = std::make_shared<Rest::CustomRouter>();
  Rest::UserController userController(router);

  ASSERT_TRUE(true);
}

TEST(UserControllerTest, handle_requests) {
  int portNumber = FLAGS_port + 4;
  Common::Database::init("user_controller_test.db");
  Pistache::Port port(portNumber);
  Pistache::Address addr(Pistache::Ipv4::any(), port);
  Rest::MainController mainController(addr, 1, false);
  std::packaged_task<void()> task([&]() { mainController.start(); });
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  auto future = task.get_future();
  std::thread(std::move(task)).detach();

  {
    Common::Models::LoginRequest loginRequest = {
        .username = "user",
        .password = "pass",
    };
    auto resp = postRequest(portNumber, "/usager/login", Common::Models::toStr(loginRequest));
    ASSERT_EQ(resp, zmqNotInit);
  }

  {
    auto resp = postRequest(portNumber, "/usager/logout", "");
    ASSERT_EQ(resp, "");
  }

  {
    Common::Models::PasswordRequest passwordRequest = {
        .oldPwd = "equipe01",
        .newPwd = "equipe1",
    };
    auto resp = postRequest(portNumber, "/usager/motdepasse", Common::Models::toStr(passwordRequest));
    ASSERT_EQ(resp, zmqNotInit);
  }

  {
    Common::Models::LoginRequest loginRequest = {
        .username = "user",
        .password = "pass",
    };
    auto resp = postRequest(portNumber, "/usager/register", Common::Models::toStr(loginRequest));
    ASSERT_EQ(resp, zmqNotInit);
  }

  auto status = future.wait_for(std::chrono::milliseconds(300));
  ASSERT_EQ(status, std::future_status::timeout);
}
