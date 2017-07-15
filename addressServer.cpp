#include <string>
#include <iostream>
#include <random>
#include <chrono>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/client.hpp>
#include <zmq.hpp>

#include "wallet.h"
using namespace bc;

const int port = 49529; // randomly selected from unreserved range

std::mt19937 *seedRandom()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    return new std::mt19937(seed);
}

void startServer(Wallet *wallet)
{
    std::mt19937 *rand_gen = seedRandom();
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:" + std::to_string(port));

    while (true) {
        zmq::message_t request;

        socket.recv(&request);

        // Send random child address as string
        // According to zmq convention, don't send null byte
        std::string address = wallet->childAddress((*rand_gen)()).encoded();
        zmq::message_t reply(address.size()-1);
        address.copy((char*)reply.data(), address.size()-1);
        socket.send(reply);
    }
}

int main(int argc, char *argv[])
{
    Wallet *wallet;

    if (argc < 2)
    {
        std::cout << "Creating new wallet" << std::endl;
        wallet = new Wallet();
    } else
    {
        std::cout << "Loading wallet from " << argv[1] << std::endl;
        wallet = new Wallet(std::string(argv[1]));
    }

    startServer(wallet);

    return 0;
}
