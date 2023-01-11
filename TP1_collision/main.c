#include <stdlib.h>
#include <stdio.h>
#include <openssl/sha.h>
#define HS 20 // HS (Hash Size): 20 pour SHA1, 32 pour SHA256

int main(){
    SHA_CTX ctx;   //contexte contenant les informations du calcul de hash en cours
    unsigned char md[20];   //contiendra un haché
    char tab[30];
    //...
    if(SHA1_Init(&ctx) == 0){
        printf("Erreur d'initialisation de contexte SHA1\n");
        return EXIT_FAILURE;
    }
    // ...
     if(SHA1_Update(&ctx,tab,30) == 0){
        printf("Erreur calcul SHA1 \n");
        return EXIT_FAILURE;
    }
    //...
    if(SHA1_Final(md,&ctx) == 0){
        printf("Erreur Finalisation SHA1\n");
        return EXIT_FAILURE;
    }
    //md contient maintenant les octets du hash de tab
    //...
    return EXIT_SUCESS;
}
