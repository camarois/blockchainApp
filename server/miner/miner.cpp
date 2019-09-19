#include <string>
#include <zmq.hpp>
#include "example.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);
    socket.connect ("tcp://localhost:5555");

    while (true) {
        std::string message = "from miner";
        zmq::message_t request(message.length());
        memcpy(request.data(), message.data(), message.length());
        socket.send(request);

        zmq::message_t reply;
        socket.recv(&reply);
        auto str = std::string(static_cast<char*>(reply.data()), reply.size());
        std::cout << str << std::endl;
    }
    return 0;

    // zmq::context_t ctx;
    // zmq::socket_t sock(ctx, zmq::socket_type::push);
    // sock.bind("inproc://test");
    // sock.send(zmq::str_buffer("Hello, world"), zmq::send_flags::dontwait);
    // Example example;
    // std::cout << "Fin: " << example.example() << std::endl;
}
