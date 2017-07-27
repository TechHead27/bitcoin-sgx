/*
 * Copied from github.com/AaronJaramillo/LibbitcoinTutorial/HDKeychain/HD_WalletTESTNET.cpp
 */
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/client.hpp>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "wallet.h"
using namespace bc;

Wallet::Wallet()
{
    entropy = data_chunk(16);
    pseudo_random_fill(entropy);
    mnemonic = wallet::create_mnemonic(entropy);
    seed = to_chunk(wallet::decode_mnemonic(mnemonic));
    privateKey = wallet::hd_private(seed, wallet::hd_private::testnet);
    publicKey = privateKey.to_public();
}

Wallet::Wallet(const data_chunk Userentropy)
{
    entropy = Userentropy;
    mnemonic = wallet::create_mnemonic(entropy);
    seed = to_chunk(wallet::decode_mnemonic(mnemonic));
    privateKey = wallet::hd_private(seed, wallet::hd_private::testnet);
    publicKey = privateKey.to_public();
}

Wallet::Wallet(const wallet::word_list mnemonicSeed)
{
    seed = to_chunk(wallet::decode_mnemonic(mnemonicSeed));
    //seed = to_chunk(hashSeed);
    mnemonic = mnemonicSeed;
    privateKey = wallet::hd_private(seed, wallet::hd_private::testnet);
    publicKey = privateKey.to_public();
}

Wallet::Wallet(const std::string& filename)
{
    std::string mnemonic;
    std::ifstream in(filename);

    in >> mnemonic;
    Wallet(split(mnemonic));
}

//display output
void Wallet::dumpKeys()
{
    displayMnemonic();
    displayPrivateKey();
    displayChildPrivateKey(1);
    displayAddress(1);

}
void Wallet::displayPrivateKey()
{
    std::cout << "\nPrivate Key:" << privateKey.encoded() << std::endl;
}

void Wallet::displayMnemonic(std::ostream& out)
{
    if(wallet::validate_mnemonic(mnemonic))
    {
        out << "\n" << join(mnemonic) << std::endl;

    }else{
        out << "\n" << "Mnemonic Invalid! " << std::endl;
    }
}

void Wallet::displayChildPrivateKey(int index)
{
    std::cout << "\nChild Key: " << childPrivateKey(index).encoded() << std::endl;
}

void Wallet::displayAddress(int index)
{
    std::cout << "\nAddress: " << childAddress(index).encoded() << std::endl;
}
void Wallet::addressRange(int start, int end)
{
    while(start != end)
    {
        displayAddress(start);
        start++;
    }
}

uint64_t Wallet::getBalance(int index)
{
    return Wallet::getBalance(childAddress(index));
}

uint64_t Wallet::getBalance(wallet::payment_address address)
{
    uint64_t unspent_balance;
    client::connection_type connection = {};
    connection.retries = 3;
    connection.timeout_seconds = 8;
    connection.server = config::endpoint("tcp://testnet.libbitcoin.net:19091");

    client::obelisk_client client(connection);


    static const auto on_done = [&unspent_balance](const std::vector<client::history>& rows)
    {
        unspent_balance = 0;

        for(const auto& row: rows)
        {

            // spend unconfirmed (or no spend attempted)
            if (row.spend.hash() == null_hash)
                unspent_balance += row.value;
        }

        std::cout<< encode_base10(unspent_balance, 8) << std::endl;

    };
    static const auto on_error2 = [](const code ec) {

        std::cout << "Error Code: " << ec.message() << std::endl;

    };


    if(!client.connect(connection))
    {
        std::cout << "Fail" << std::endl;
    } else {
        std::cout << "Connection Succeeded" << std::endl;
    }

    // TODO: Check from height on intialization to be faster?
    client.blockchain_fetch_history3(on_error2, on_done, address, 0);
    client.wait();
    return unspent_balance;
}

//accesor
wallet::hd_private Wallet::childPrivateKey(uint32_t index)
{
    if (index >= (1 << 31)) // doesn't seem to work when this bit is set
        index &= ~(1 << 31);
    return privateKey.derive_private(index);
}

wallet::hd_public Wallet::childPublicKey(uint32_t index)
{
    if (index >= (1 << 31)) // doesn't seem to work when this bit is set
        index &= ~(1 << 31);
    return publicKey.derive_public(index);
}

wallet::payment_address Wallet::childAddress(uint32_t index)
{
    return wallet::payment_address(wallet::ec_public(childPublicKey(index).point()), 0x6f);
}
