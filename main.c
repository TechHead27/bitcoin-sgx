#include <stdio.h>

#include "encrypt.h"

int main(int argc, char **argv) {
	unsigned char *key, *iv;
	FILE *keyfile;

	encryptFiles(argv[1], &key, &iv);
	keyfile = fopen("keyfile", "wb");
	fwrite(key, 1, KEY_SIZE, keyfile);
	fwrite(iv, 1, BLOCK_SIZE, keyfile);

	return 0;
}
