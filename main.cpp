#include <cstdio>

#include "encrypt.h"

int main(int argc, char **argv) {
	Crypt testCrypt;

	testCrypt.encryptFiles(argv[1]);

	return 0;
}
