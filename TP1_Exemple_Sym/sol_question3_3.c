#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

#define DES_BLOCK_SIZE 8

void char_table_to_hex_string(u_char *table, char *hex_str, int table_size) {
    int i;
    for (i = 0; i < table_size; i++) {
        sprintf(hex_str + (i * 2), "%02x", table[i]);
    }
}

int cmp(u_char *c1, u_char *c2){
    for(int i = 0; i < DES_BLOCK_SIZE; i++){
        if( *(c1 + i) != *(c2 + i)){
            return -1;
        }
    }
    return 0;
}

int main(){
    u_char double_ciphered[DES_BLOCK_SIZE]={0x4a, 0xf4, 0xef, 0x60, 0xc0, 0x11, 0x71, 0x07};
    u_char plaintext[DES_BLOCK_SIZE] = {0x3e, 0x9a, 0x3e, 0x26, 0x5d, 0xcb, 0x6b, 0xa7};

    u_char key1[DES_BLOCK_SIZE] = {0x57, 0x10, 0x5c, 0x17, 0x72, 0xbf, 0x6c, 0xc8};
    u_char key2[DES_BLOCK_SIZE] = {0x79, 0xb0, 0x09, 0x0b, 0x28, 0x52, 0x36, 0x2e};

    // Test Values:
    u_char check_plaintext[DES_BLOCK_SIZE] = "ItWorks";
    u_char check_result[DES_BLOCK_SIZE] = {0xc9,0xbf,0x10,0x3a,0xcc,0x1c,0xad,0xf5};
    
    u_char deciphered1[DES_BLOCK_SIZE];
    u_char deciphered2[DES_BLOCK_SIZE + 1];
  
    u_char ciphered1[DES_BLOCK_SIZE];
    u_char ciphered2[DES_BLOCK_SIZE];
    
    int outl;
     
    deciphered2[DES_BLOCK_SIZE] = '\0';

    EVP_CIPHER_CTX *dec1 = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX *dec2 = EVP_CIPHER_CTX_new();

    EVP_CIPHER_CTX *enc1 = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX *enc2 = EVP_CIPHER_CTX_new();

    EVP_DecryptInit(dec1, EVP_des_cbc(), key1, NULL); 
    EVP_CIPHER_CTX_set_padding(dec1, 0);
    EVP_DecryptInit(dec2, EVP_des_cbc(), key2, NULL); 
    EVP_CIPHER_CTX_set_padding(dec2, 0);

    if (EVP_DecryptUpdate(dec2, deciphered1, &outl, double_ciphered, DES_BLOCK_SIZE) == 0){
        printf("ERROR decRYPTING\n");
        goto exit;
    }

    if(EVP_DecryptFinal(dec2, deciphered1, &outl) == 0){
        printf("ERROR DECRYPTING\n");
        goto exit;
    }

    if (EVP_DecryptUpdate(dec1, deciphered2, &outl, deciphered1, DES_BLOCK_SIZE) == 0){
        printf("ERROR decRYPTING\n");
        goto exit;
    }

    if(EVP_DecryptFinal(dec1, deciphered2, &outl) == 0){
        printf("ERROR DECRYPTING\n");
        goto exit;
    }
    
   printf("OG MSG: %s\n", deciphered2);

    EVP_EncryptInit(enc1, EVP_des_cbc(), key1, NULL); 
    EVP_CIPHER_CTX_set_padding(enc1, 0);
    EVP_EncryptInit(enc2, EVP_des_cbc(), key2, NULL); 
    EVP_CIPHER_CTX_set_padding(enc2, 0);

    if (EVP_EncryptUpdate(enc1, ciphered1, &outl, check_plaintext, DES_BLOCK_SIZE) == 0){
        printf("ERROR ENRYPTING 1\n");
        goto exit;
    }

    if(EVP_EncryptFinal(enc1, ciphered1, &outl) == 0){
        printf("ERROR ENCRYPTING 2\n");
        goto exit;
    }

    if (EVP_EncryptUpdate(enc2, ciphered2, &outl, ciphered1, DES_BLOCK_SIZE) == 0){
        printf("ERROR ENRYPTING 3\n");
        goto exit;
    }

    if(EVP_EncryptFinal(enc2, ciphered2, &outl) == 0){
        printf("ERROR DECRYPTING 4\n");
        goto exit;
    }
    
   printf("cmp results: %d\n", cmp(ciphered2, check_result));

exit:
    EVP_CIPHER_CTX_free(dec2);
    EVP_CIPHER_CTX_free(dec1);
    EVP_CIPHER_CTX_free(enc2);
    EVP_CIPHER_CTX_free(enc1);
    return EXIT_SUCCESS;
}
