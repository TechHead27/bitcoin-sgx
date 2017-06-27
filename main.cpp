#include <cstdio>

#include "encrypt.h"

int main(int argc, char **argv) {
	Crypt *testCrypt;

	if (argc >= 3)
		testCrypt = new Crypt(argv[2]);
	else
		testCrypt = new Crypt();

	testCrypt->encryptFiles(argv[1]);

	delete testCrypt;
	return 0;
}
