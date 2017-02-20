#ifndef ENCRYPT_H
#define ENCRYPT_H

#define BLOCK_SIZE 16
#define KEY_SIZE 32

int encryptFiles(const char *dir, unsigned char **key, unsigned char **iv);

#endif
