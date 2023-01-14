#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <errno.h>
#include <string.h>

#define DES_BLOCK_SIZE 8

void char_table_to_hex_string(u_char *table, char *hex_str, int table_size) {
    int i;
    for (i = 0; i < table_size; i++) {
        sprintf(hex_str + (i * 2), "%02x", table[i]);
    }
}

int main(){
    u_char double_ciphered[8]={0x4a, 0xf4, 0xef, 0x60, 0xc0, 0x11, 0x71, 0x07};
    u_char plaintext[8] = {0x3e, 0x9a, 0x3e, 0x26, 0x5d, 0xcb, 0x6b, 0xa7};
    u_char key1[DES_BLOCK_SIZE] = {87,16,92,23,114,191,0,0};
    u_char key2[DES_BLOCK_SIZE] = {121,176,9,11,40,82,0,0};

    // Test Values:
    u_char check_plaintext[8] = "ItWorks";
    u_char check_result[8] = {0xc9,0xbf,0x10,0x3a,0xcc,0x1c,0xad,0xf5};
    
    u_char ciphered[DES_BLOCK_SIZE];
    char *str_ciphered = malloc(2 * DES_BLOCK_SIZE);
    char *str_key1 = malloc(2 * DES_BLOCK_SIZE);
    int outl;
    
    FILE *f = fopen("hashes_keys", "w+");
    if(f == NULL){
        printf("ERROR FOPEN\n");
        return 1;
    }

    EVP_CIPHER_CTX *enc = EVP_CIPHER_CTX_new();

    for(ushort i = 0; i < USHRT_MAX; i++){

        key1[7] = (u_char)(i & 0x00ff);
        key1[6] = (u_char)((i >> 8) & 0x00ff);

        //printf("%x %x %x\n", key1[6], key1[7], j );
        EVP_EncryptInit(enc, EVP_des_cbc(), key1, NULL); 
        EVP_CIPHER_CTX_set_padding(enc, 0);

        if (EVP_EncryptUpdate(enc, ciphered, &outl, plaintext, DES_BLOCK_SIZE) ==0){
            printf("ERROR ENCRYPTING\n");
            EVP_CIPHER_CTX_free(enc);
            return EXIT_FAILURE;
        }

        if(EVP_EncryptFinal(enc, ciphered, &outl)==0){
            printf("ERROR DECRYPTING\n");
            EVP_CIPHER_CTX_free(enc);
            return EXIT_FAILURE;
        }
        // write o the file
        char_table_to_hex_string(ciphered, str_ciphered, DES_BLOCK_SIZE);
        char_table_to_hex_string(key1, str_key1, DES_BLOCK_SIZE);

        fwrite(str_ciphered, 1, 2 * DES_BLOCK_SIZE, f);
        fwrite(" ", 1, 1, f);
        fwrite(str_key1, 1, 2 * DES_BLOCK_SIZE, f);
        fwrite("\n", 1, 1, f);
        memset(str_ciphered, '\0', 2 * DES_BLOCK_SIZE);
        memset(str_key1, '\0', 2 * DES_BLOCK_SIZE);

    
    }
    EVP_CIPHER_CTX_free(enc);
    free(str_ciphered);
    free(str_key1);
    fclose(f);

    return EXIT_SUCCESS;

}
