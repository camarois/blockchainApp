#include "rest_utils.hpp"
#include <chrono>
#include <common/models.hpp>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <future>
#include <gflags/gflags.h>
#include <gtest/gtest.h>
#include <rest/custom_router.hpp>
#include <rest/info_controller.hpp>
#include <rest/main_controller.hpp>
#include <rest/zmq.hpp>
#include <thread>
#include <iostream>
#include <experimental/filesystem>
#include <fstream>
#include <common/base64.hpp>

DECLARE_int32(FLAGS_port);

TEST(FileControllerTest, start) {
  std::shared_ptr<Rest::CustomRouter> router = std::make_shared<Rest::CustomRouter>();
  Rest::FileController fileController(router);

  ASSERT_TRUE(true);
}

TEST(FileControllerTest, handle_requests) {
  int portNumber = FLAGS_port + 1;
  Common::Database::init("file_controller_test.db");
  Pistache::Port port(portNumber);
  Pistache::Address addr(Pistache::Ipv4::any(), port);
  Rest::MainController mainController(addr, 1, false);
  std::packaged_task<void()> task([&]() { mainController.start(); });
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  auto future = task.get_future();
  std::thread(std::move(task)).detach();

  {
    std::filesystem::create_directory("transactions");
    std::ofstream out("transactions/2-inf3995.pdf");
    out << "content";
    out.close();
    Common::Models::GradesRequest gradesRequest = {
        .acronym = "inf3995",
        .trimester = 2,
    };
    auto resp = postRequest(portNumber, "/fichier/notes", Common::Models::toStr(gradesRequest));
    ASSERT_EQ(resp, Common::Base64::encode("content"));
  }

  auto status = future.wait_for(std::chrono::milliseconds(300));
  ASSERT_EQ(status, std::future_status::timeout);
}
