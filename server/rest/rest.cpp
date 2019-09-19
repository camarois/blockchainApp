#include <example.hpp>
#include <string>
#include <iostream>
#include <unistd.h>
#include <zmq.hpp>

int main(int argc, char *argv[]) {
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind ("tcp://*:5555");

    while (true) {
        zmq::message_t request;

        //  Wait for next request from client
        socket.recv (&request);
        std::cout << "Received Hello" << std::endl;

        //  Do some 'work'
        sleep(1);

        //  Send reply back to client
        zmq::message_t reply (5);
        memcpy (reply.data (), "World", 5);
        socket.send (reply);
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
