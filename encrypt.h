#pragma once

extern "C" {
#include <openssl/evp.h>
#include <ftw.h>
}

#define BLOCK_SIZE 16
#define KEY_SIZE 32

class Crypt {
	private:
		unsigned char key[KEY_SIZE];
		unsigned char iv[BLOCK_SIZE];
		int enc;

		void shouldEncrypt(const char *name, int flag, struct FTW *path);
		void encryptFile(const char *name);
		void handleErrors(int error);

		static Crypt *current;
		static int encryptDir(const char *name, const struct stat *stats, int flag, struct FTW *path);
	public:
		Crypt();
		Crypt(char* keyfileName);
		int encryptFiles(const char *dir);
};
