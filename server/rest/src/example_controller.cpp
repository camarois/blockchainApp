#include <common/message_helper.hpp>
#include <rest/example_controller.hpp>

namespace Rest {

ExampleController::ExampleController(const std::shared_ptr<Pistache::Rest::Router>& router)
    : context_(1),
      socket_(context_, ZMQ_REP) {
  socket_.bind("tcp://*:5555");
  std::cout << "zmq socket created" << std::endl;
  setupRoutes(router);
}

void ExampleController::setupRoutes(const std::shared_ptr<Pistache::Rest::Router>& router) {
  Pistache::Rest::Routes::Get(*router, "/start",
			      Pistache::Rest::Routes::bind(&ExampleController::handleStart, this));
}

void ExampleController::handleStart(const Pistache::Rest::Request& /*unused*/,
				  Pistache::Http::ResponseWriter response) {
  for (int i = 0; i < 3; ++i) {
    zmq::message_t reply;
    std::cout << "Receving " << std::flush;
    socket_.recv(reply, zmq::recv_flags::none);
    auto str = Common::MessageHelper::toString(reply);
    std::cout << str << std::endl;

    sleep(1);

    std::cout << "Sending " << std::flush;
    auto request = Common::MessageHelper::fromString("from rest");
    socket_.send(request, zmq::send_flags::none);
    std::cout << "Sent " << std::flush;
  }

  response.send(Pistache::Http::Code::Ok, "Fake mining completed.");
}

} // namespace Rest
