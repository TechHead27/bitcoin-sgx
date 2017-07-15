#include <cstdio>

#include "encrypt.h"

int main(int argc, char **argv) {
	Crypt *testCrypt;

	if (argc >= 3)
		testCrypt = new Crypt(argv[2]);
	else
		testCrypt = new Crypt();

	if (testCrypt->encryptFiles(argv[1]) < 0)
		perror(NULL);

	delete testCrypt;
	return 0;
}
