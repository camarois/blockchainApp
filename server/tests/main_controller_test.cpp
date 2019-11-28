#include <chrono>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <future>
#include <gtest/gtest.h>
#include <rest/main_controller.hpp>
#include "rest_utils.hpp"
#include <thread>

TEST(MainControllerTest, start_main_controller) {
  Pistache::Port port(8080);
  Pistache::Address addr(Pistache::Ipv4::any(), port);

  Rest::MainController mainController(addr, 1, false);
  std::packaged_task<void()> task([&]() { mainController.start(); });
  auto future = task.get_future();
  std::thread(std::move(task)).detach();
  auto status = future.wait_for(std::chrono::milliseconds(300));
  ASSERT_EQ(status, std::future_status::timeout);
}
