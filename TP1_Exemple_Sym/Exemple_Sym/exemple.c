#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>

#define DES_BLOCK_SIZE 8

int main()
{
        unsigned char plaintext[8] ="Exemple";
        unsigned char ciphered[8];
        unsigned char deciphered[8];

        int i,outl;

        /*Création des contextes de chiffrement et déchiffrement)*/
        EVP_CIPHER_CTX *ctx_enc = EVP_CIPHER_CTX_new();
        EVP_CIPHER_CTX *ctx_dec = EVP_CIPHER_CTX_new();

        /*Clé symétrique*/
        unsigned char key[] = {0,1,2,3,4,5,6,7};

        /*Initialisation des contextes de chiffrement et déchiffrement*/
        EVP_EncryptInit(ctx_enc, EVP_des_cbc(),key,NULL); //Chiffrement en des_cbc, avec la clé key (pas d'IV fourni)
        EVP_DecryptInit(ctx_dec, EVP_des_cbc(),key,NULL); //Dechiffrement
        EVP_CIPHER_CTX_set_padding(ctx_enc,0); //On désactive le padding : les données auront toujours
        EVP_CIPHER_CTX_set_padding(ctx_dec,0); //la bonne taille dans les exercices proposés

        /** EXEMPLE DE CHIFFREMENT**/
        /*Chiffrement d'un (nouveau) bloc*/
        if( EVP_EncryptUpdate(ctx_enc, ciphered,&outl,plaintext,DES_BLOCK_SIZE) == 0){
            printf("Erreur dans le chiffrement\n");
            EVP_CIPHER_CTX_free(ctx_dec);
            EVP_CIPHER_CTX_free(ctx_enc);
            return EXIT_FAILURE;
        }

        /*Finalisation Chiffrement */
        if(EVP_EncryptFinal(ctx_enc, ciphered, &outl)==0){ //Note: ne change rien ici car le padding est désactivé
            printf("Erreur dans le chiffrement\n");
            EVP_CIPHER_CTX_free(ctx_dec);
            EVP_CIPHER_CTX_free(ctx_enc);
            return EXIT_FAILURE;
        }

        /*Affichage en hexa des octets du chiffré*/
        printf("Octets du chiffré: ");
        for(i=0;i<8;i++){
            printf("%x ", ciphered[i]);
        }
        printf("\n");

        /** EXEMPLE DE DÉCHIFFREMENT**/

        /*Déchiffrement d'un (nouveau) bloc*/
        if (EVP_DecryptUpdate(ctx_dec, deciphered,&outl,ciphered,DES_BLOCK_SIZE) ==0){
            printf("Erreur dans le déchiffrement\n");
            EVP_CIPHER_CTX_free(ctx_dec);
            EVP_CIPHER_CTX_free(ctx_enc);
            return EXIT_FAILURE;
        }

        /*Finalisation Déchiffrement*/
        if(EVP_DecryptFinal(ctx_dec, deciphered, &outl)==0){
            printf("Erreur dans le déchiffrement\n");
            EVP_CIPHER_CTX_free(ctx_dec);
            EVP_CIPHER_CTX_free(ctx_enc);
            return EXIT_FAILURE;
        }

        printf("Octets déchiffrés: ");
        for(i=0;i<8;i++){
            printf("%x ", deciphered[i]);
        }
        printf("\n");

        printf("Chaine de caractères obtenue après déchiffrement: %s \n",deciphered);

        EVP_CIPHER_CTX_free(ctx_dec);
        EVP_CIPHER_CTX_free(ctx_enc);

        return EXIT_SUCCESS;

}
