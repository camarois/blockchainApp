#include <string>
#include <zmq.hpp>
#include "example.hpp"
#include <iostream>
#include <unistd.h>

int main() {
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);
    socket.connect("tcp://localhost:5555");

    while (true) {
        std::string message = "from miner";
        zmq::message_t request(message.length());
        memcpy(request.data(), message.data(), message.length());
        socket.send(request, zmq::send_flags::none);

        zmq::message_t reply;
        socket.recv(reply, zmq::recv_flags::none);
        auto str = std::string(static_cast<char*>(reply.data()), reply.size());
        std::cout << str << std::endl;
    }
    return 0;
}
