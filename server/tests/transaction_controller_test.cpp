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
#include <rest/transaction_controller.hpp>
#include <rest/zmq.hpp>
#include <thread>

DECLARE_int32(FLAGS_port);

TEST(TransactionControllerTest, start) {
  std::shared_ptr<Rest::CustomRouter> router = std::make_shared<Rest::CustomRouter>();
  Rest::AdminController adminController(router);

  ASSERT_TRUE(true);
}

TEST(TransactionControllerTest, handle_requests) {
  int portNumber = FLAGS_port + 3;
  Common::Database::init("transaction_controller_test.db");
  Pistache::Port port(portNumber);
  Pistache::Address addr(Pistache::Ipv4::any(), port);
  Rest::MainController mainController(addr, 1, false);
  std::packaged_task<void()> task([&]() { mainController.start(); });
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  auto future = task.get_future();
  std::thread(std::move(task)).detach();

  {
    Common::Models::TransactionRequest transactionRequest = {
        .acronym = "inf3995", .name = "projet", .trimester = 0, .base64Pdf = "asgagag"};
    auto resp = postRequest(portNumber, "/transaction", Common::Models::toStr(transactionRequest));
    ASSERT_EQ(resp, zmqNotInit);
  }

  auto status = future.wait_for(std::chrono::milliseconds(300));
  ASSERT_EQ(status, std::future_status::timeout);
}
