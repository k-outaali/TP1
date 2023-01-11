#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#define LEN 422435
#define HS 20 //taille en octets du hash pour SHA1

void affiche_hash_hex(unsigned char* md, int size){
    int i;
    for(i=0;i<size;i++){
        printf("%02x ",md[i]);
    }
    printf("\n");

}

void affiche_hash(unsigned char* md, int size){
    int i;
    for(i=0;i<size;i++){
        printf("%d ",md[i]);
    }
    printf("\n");

}


int main()
{
    SHA_CTX ctx;
    unsigned char md[HS];
    char *str;
    str = (char *) malloc(sizeof(char) * LEN);
    if(str == NULL){
        printf("malloc failed\n");
        return 1;
    }
    FILE *f1;
    f1 = fopen("collision1.pdf", "rb");
    if(f1 == NULL){
        printf("failed to open file1\n");
        return 1;
    }
    if(SHA1_Init(&ctx) == 0){
        printf("Erreur Init\n");
        return 1;
    }
    while(fread(str, 1, 1, f1) == 1){
        if(SHA1_Update(&ctx, str, 1) == 0){
            printf("Erreur Update\n");
            return 1;
        }
    }
    if(SHA1_Final(md,&ctx)== 0){
        printf("Erreur Final\n");
        return 1;
    }


    affiche_hash(md,HS);
    affiche_hash_hex(md,HS);

    f1 = fopen("collision2.pdf", "rb");
    if(f1 == NULL){
        printf("failed to open file1\n");
        return 1;
    }
    if(SHA1_Init(&ctx) == 0){
        printf("Erreur Init\n");
        return 1;
    }
    while(fread(str, 1, 1, f1) == 1){
        if(SHA1_Update(&ctx, str, 1) == 0){ //4 peut ici être remplacé par strlen(str)
            printf("Erreur Update\n");
            return 1;
        }
    }
    if(SHA1_Final(md,&ctx)== 0){
        printf("Erreur Final\n");
        return 1;
    }


    affiche_hash(md,HS);
    affiche_hash_hex(md,HS);

    return 0;
}
