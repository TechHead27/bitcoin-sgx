#include <bitcoin/bitcoin.hpp>
#include <bitcoin/client.hpp>
#include <string.h>
#include <iostream>
using namespace bc;

class Wallet
{
public:
	
	//constructor
	Wallet();

	Wallet(const data_chunk Userentropy);

	Wallet(const wallet::word_list mnemonicSeed);

    Wallet(const std::string& filename);

	//display output
	void dumpKeys();
	void displayPrivateKey();

	void displayMnemonic(std::ostream& out = std::cout);

	void displayChildPrivateKey(int index);

	void displayAddress(int index);
	void addressRange(int start, int end);

	void getBalance(int index);

	//accesor
	wallet::hd_private childPrivateKey(int index);

	wallet::hd_public childPublicKey(int index);

	wallet::payment_address childAddress(int index);

private:
	//members
	data_chunk entropy;
	data_chunk seed;
	wallet::word_list mnemonic;
	wallet::hd_private privateKey;
	wallet::hd_public publicKey;

};
