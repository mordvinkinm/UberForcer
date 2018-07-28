/**************************************************************************
*            Unix-like crypt(3) Algorithm for Password Encryption
*
*   File    : crypt3.h
*   Purpose : Provides crypt(3) functionality to ANSI C compilers
*             without a need for the crypt library.
*
***************************************************************************/

#ifndef CRYPT_3
#define CRYPT_3

#define CRYPT_HASH_SIZE (16)

/**************************************************************************
*
* Description: Unix crypt(3) function.
*
* Inputs:      char *pw
*              Pointer to 8 character encryption key (user password)
*
*              char *salt
*              Pointer to 2 character salt used to modify the DES results.
*
* Returns:     Array containing the salt concatenated
*              on to the encrypted results.
**************************************************************************/
char *crypt(char *pw, char *salt);

/**************************************************************************
* Function:    crypt_r
*
* Description: Thread-safe version of crypt function
*
* Inputs:      char *pw
*              Pointer to 8 character encryption key (user password)

*              char *salt
*              Pointer to 2 character salt used to modify the DES results.

*              char (*iobuf)[16]
*              Pointer to 16 character buffer for output
*
* Returns:     Pointer to the first element of *iobuf input buffer
**************************************************************************/
char *crypt_r(char *pw, char *salt, char (*iobuf)[CRYPT_HASH_SIZE]);

#endif /* CRYPT_3 */