#include <stdlib.h>
#include <stdio.h>

#include "crypt3.h"

void encrypt_routine()
{
    char password[50];
    char salt[50];

    printf("Password: ");
    scanf("%s", password);
    
    printf("Salt: ");
    scanf("%s", salt);

    char *encrypted = crypt(password, salt);
    printf("%s", encrypted);
}

int main(int argc, char *argv[])
{
    encrypt_routine();

    return EXIT_SUCCESS;
}