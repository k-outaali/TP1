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

int main(){
    u_char double_ciphered[8]={0x4a, 0xf4, 0xef, 0x60, 0xc0, 0x11, 0x71, 0x07};
    u_char plaintext[8] = {0x3e, 0x9a, 0x3e, 0x26, 0x5d, 0xcb, 0x6b, 0xa7};
    u_char key1[DES_BLOCK_SIZE] = {87,16,92,23,114,191,0,0};
    u_char key2[DES_BLOCK_SIZE] = {121,176,9,11,40,82,0,0};

    // Test Values:
    u_char check_plaintext[8] = "ItWorks";
    u_char check_result[8] = {0xc9,0xbf,0x10,0x3a,0xcc,0x1c,0xad,0xf5};
    
    u_char deciphered[DES_BLOCK_SIZE];
    char *str_deciphered = malloc(2 * DES_BLOCK_SIZE + 1);
    char *str_key2 = malloc(2 * DES_BLOCK_SIZE);
    char *line = malloc(DES_BLOCK_SIZE * DES_BLOCK_SIZE);
    char *stored_cipher = malloc(2 * DES_BLOCK_SIZE + 1);
  
    int outl;
    
    FILE *f = fopen("hashes_keys", "r+");
    if(f == NULL){
        printf("ERROR FOPEN\n");
        return 1;
    }

    EVP_CIPHER_CTX *dec = EVP_CIPHER_CTX_new();

    for(ushort i = 0; i < USHRT_MAX; i++){

        key2[7] = (u_char)(i & 0x00ff);
        key2[6] = (u_char)((i >> 8) & 0x00ff);
        EVP_DecryptInit(dec, EVP_des_cbc(), key2, NULL); 
        EVP_CIPHER_CTX_set_padding(dec, 0);

        if (EVP_DecryptUpdate(dec, deciphered, &outl, double_ciphered, DES_BLOCK_SIZE) == 0){
            printf("ERROR decRYPTING\n");
            goto exit;
        }

        if(EVP_DecryptFinal(dec, deciphered, &outl) == 0){
            printf("ERROR DECRYPTING\n");
            goto exit;
        }

        memset(str_deciphered, '\0', 2 * DES_BLOCK_SIZE);
        memset(str_key2, '\0', 2 * DES_BLOCK_SIZE);
        

        char_table_to_hex_string(deciphered, str_deciphered, DES_BLOCK_SIZE);
        char_table_to_hex_string(key2, str_key2, DES_BLOCK_SIZE);

        for(ushort j = 0; j < USHRT_MAX; j++){

            if(fread(line, 1, 34, f) != 34){
                break;
            }
            strncpy(stored_cipher, line, 16);
            if(strcmp(stored_cipher, str_deciphered) == 0){
                printf("found key2=%s key1=%s", str_key2, line + 17);
                
            }
            memset(line, '\0', DES_BLOCK_SIZE * DES_BLOCK_SIZE);
        }
        rewind(f);
        
    }
exit:
    EVP_CIPHER_CTX_free(dec);
    free(str_deciphered);
    free(str_key2);
    free(line);
    free(stored_cipher);
    fclose(f);

    return EXIT_SUCCESS;

}
