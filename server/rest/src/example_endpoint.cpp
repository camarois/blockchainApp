#include <common/message_helper.hpp>
#include <rest/example_endpoint.hpp>

using json = nlohmann::json;

ExampleEndpoint::ExampleEndpoint(Pistache::Address addr)
    : httpEndpoint_(addr), context_(1), socket_(context_, ZMQ_REP) {
  socket_.bind("tcp://*:5555");
  std::cout << "zmq socket created" << std::endl;
}

void ExampleEndpoint::init(size_t thr) {
  auto opts = Pistache::Http::Endpoint::options().threads(thr);
  httpEndpoint_.init(opts);
  setupRoutes();
}

void ExampleEndpoint::start() {
  httpEndpoint_.setHandler(router_.handler());
  httpEndpoint_.serve();
}

void ExampleEndpoint::setupRoutes() {
  Pistache::Rest::Routes::Get(router_, "/lol",
			      Pistache::Rest::Routes::bind(&ExampleEndpoint::handleLol, this));

  Pistache::Rest::Routes::Get(router_, "/start",
			      Pistache::Rest::Routes::bind(&ExampleEndpoint::handleStart, this));
}

void ExampleEndpoint::handleLol(const Pistache::Rest::Request& /*unused*/,
				Pistache::Http::ResponseWriter response) {
  static int lol = 0;
  Example example;
  json j = {{"lol", lol++}, {"example", example.example()}};
  response.send(Pistache::Http::Code::Ok, j.dump(4));
}

void ExampleEndpoint::handleStart(const Pistache::Rest::Request& /*unused*/,
				  Pistache::Http::ResponseWriter response) {
  for (int i = 0; i < 3; ++i) {
    zmq::message_t reply;
    std::cout << "Receving " << std::flush;
    socket_.recv(reply, zmq::recv_flags::none);
    auto str = MessageHelper::toString(reply);
    std::cout << str << std::endl;

    sleep(1);

    std::cout << "Sending " << std::flush;
    auto request = MessageHelper::fromString("from rest");
    socket_.send(request, zmq::send_flags::none);
    std::cout << "Sent " << std::flush;
  }

  response.send(Pistache::Http::Code::Ok, "Fake mining completed.");
}
