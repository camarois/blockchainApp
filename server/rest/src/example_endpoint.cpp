#include <common/message_helper.hpp>
#include <example_endpoint.hpp>
#include <zmq.hpp>

using json = nlohmann::json;

void ExampleEndpoint::init(size_t thr) {
  auto opts = Pistache::Http::Endpoint::options().threads(thr);
  httpEndpoint->init(opts);
  setupRoutes();
}

void ExampleEndpoint::start() {
  httpEndpoint->setHandler(router.handler());
  httpEndpoint->serve();
}

void ExampleEndpoint::setupRoutes() {
  Pistache::Rest::Routes::Get(router, "/lol",
			      Pistache::Rest::Routes::bind(&ExampleEndpoint::handleLol, this));

  Pistache::Rest::Routes::Get(router, "/start",
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
  const std::string kMessageStarting = "Starting the fake mining.";
  response.send(Pistache::Http::Code::Ok, kMessageStarting)
      .then(
	  [&](ssize_t /*unused*/) {
	    std::cout << kMessageStarting << std::endl;

	    zmq::context_t context(1);
	    zmq::socket_t socket(context, ZMQ_REP);
	    socket.bind("tcp://*:5555");

	    while (true) {
	      zmq::message_t request;
	      socket.recv(request, zmq::recv_flags::none);
	      auto str = MessageHelper::to_string(request);
	      std::cout << str << std::endl;

	      sleep(1);

	      auto reply = MessageHelper::from_string("from rest");
	      socket.send(reply, zmq::send_flags::none);
	    }
	  },
	  Pistache::Async::Throw);
}
