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

DECLARE_int32(FLAGS_port);

TEST(InfoControllerTest, start) {
  std::shared_ptr<Rest::CustomRouter> router = std::make_shared<Rest::CustomRouter>();
  Rest::InfoController infoController(router);

  ASSERT_TRUE(true);
}

TEST(InfoControllerTest, handle_requests) {
  int portNumber = FLAGS_port + 0;
  Common::Database::init("info_controller_test.db");
  Pistache::Port port(portNumber);
  Pistache::Address addr(Pistache::Ipv4::any(), port);
  Rest::MainController mainController(addr, 1, false);
  std::packaged_task<void()> task([&]() { mainController.start(); });
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  auto future = task.get_future();
  std::thread(std::move(task)).detach();

  {
    Common::Models::ClassesRequest classesRequest = {
        .acronym = "inf3995",
        .trimester = 2,
    };
    auto resp = postRequest(portNumber, "/info/cours", Common::Models::toStr(classesRequest));
    ASSERT_EQ(resp, zmqNotInit);
  }

  {
    Common::Models::StudentRequest studentRequest = {
        .acronym = "inf3995",
        .trimester = "2",
        .id = "0",
    };
    auto resp = postRequest(portNumber, "/info/etudiant", Common::Models::toStr(studentRequest));
    ASSERT_EQ(resp, zmqNotInit);
  }

  {
    auto resp = getRequest(portNumber, "/info/listeEtudiants");
    ASSERT_EQ(resp, zmqNotInit);
  }

  {
    auto resp = getRequest(portNumber, "/info/listeCours");
    ASSERT_EQ(resp, zmqNotInit);
  }

  auto status = future.wait_for(std::chrono::milliseconds(300));
  ASSERT_EQ(status, std::future_status::timeout);
}
