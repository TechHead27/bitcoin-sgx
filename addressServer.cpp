#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <signal.h>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/client.hpp>
#include <zmq.hpp>

#include "wallet.h"
#include "addressServer.h"
using namespace bc;

bool interrupted = false;

void signal_handler(int signal)
{
    interrupted = true;
}

void catch_signals()
{
    struct sigaction action;
    action.sa_handler = signal_handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
}

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

    while (!interrupted) {
        zmq::message_t request;

        try
        {
            socket.recv(&request);
        } catch (zmq::error_t e)
        {
            std::cerr << "recv interrupted" << std::endl;
            return;
        }
        std::cerr << "Recieved request" << std::endl;

        // Send random child address as string
        // According to zmq convention, don't send null byte
        std::string address = wallet->childAddress((*rand_gen)()).encoded();
        std::cerr << "Sending address " << address << std::endl;
        zmq::message_t reply(address.size());
        address.copy((char*)reply.data(), address.size());
        socket.send(reply);
    }
}

int main(int argc, char *argv[])
{
    Wallet *wallet;

    catch_signals();

    if (argc < 2)
    {
        std::cout << "Creating new wallet" << std::endl;
        wallet = new Wallet();
        std::cout << "Writing wallet to wallet.out" << std::endl;
        std::ofstream outFile("wallet.out");
        wallet->displayMnemonic(outFile);
    } else
    {
        std::cout << "Loading wallet from " << argv[1] << std::endl;
        try
        {
            wallet = new Wallet(std::string(argv[1]));
        } catch (MnemonicException e)
        {
            std::cout << e.what();
            exit(EXIT_FAILURE);
        }
    }

    startServer(wallet);

    delete wallet;

    return 0;
}
