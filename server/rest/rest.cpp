#include <example.hpp>
#include <string>
#include <iostream>
#include <unistd.h>
#include <zmq.hpp>

int main(int argc, char *argv[]) {
    zmq::context_t context(1);
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind ("tcp://*:5555");

    while (true) {
        zmq::message_t request;

        socket.recv(&request);
        auto str = std::string(static_cast<char*>(request.data()), request.size());
        std::cout << str << std::endl;

        sleep(1);

        std::string message = "from rest";
        zmq::message_t reply(message.length());
        memcpy(reply.data(), message.data(), message.length());
        socket.send(reply);
    }
    return 0;

    // Port port(10000);

    // int thr = 2;

    // if (argc >= 2) {
    //     port = std::stol(argv[1]);

    //     if (argc == 3)
    //         thr = std::stol(argv[2]);
    // }

    // Address addr(Ipv4::any(), port);

    // cout << "Cores = " << hardware_concurrency() << endl;
    // cout << "Using " << thr << " threads" << endl;

    // StatsEndpoint stats(addr);

    // stats.init(thr);
    // stats.start();
}
