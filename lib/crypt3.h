#ifndef CRYPT_3
#define CRYPT_3

#define CRYPT_HASH_SIZE (16)

char *crypt(char *pw, char *salt);

char *crypt_r(char *pw, char *salt, char (*iobuf)[CRYPT_HASH_SIZE]);

#endif /* CRYPT_3 */