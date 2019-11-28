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
#include <rest/ping_controller.hpp>
#include <rest/zmq.hpp>
#include <thread>

DECLARE_int32(FLAGS_port);

TEST(PingControllerTest, start) {
  std::shared_ptr<Rest::CustomRouter> router = std::make_shared<Rest::CustomRouter>();
  Rest::PingController pingController(router);

  ASSERT_TRUE(true);
}

TEST(PingControllerTest, handle_requests) {
  int portNumber = FLAGS_port + 5;
  Common::Database::init("ping_controller_test.db");
  Pistache::Port port(portNumber);
  Pistache::Address addr(Pistache::Ipv4::any(), port);
  Rest::MainController mainController(addr, 1, false);
  std::packaged_task<void()> task([&]() { mainController.start(); });
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  auto future = task.get_future();
  std::thread(std::move(task)).detach();

  {
    auto resp = getRequest(portNumber, "/ping");
    ASSERT_EQ(resp, "PONG");
  }

  {
    auto resp = getRequest(portNumber, "/sping");
    ASSERT_EQ(resp, "PONG");
  }

  auto status = future.wait_for(std::chrono::milliseconds(300));
  ASSERT_EQ(status, std::future_status::timeout);
}
