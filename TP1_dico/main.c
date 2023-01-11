#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>


#define HS 32
#define LEN 128



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

int read_line(FILE *file, char *s) {

    char lineBuffer[LEN];
    char ch = getc(file);
    int count = 0;

    while ((ch != '\n') && (ch != EOF)) {
        
        lineBuffer[count] = ch;
        count++;

        ch = getc(file);
    }

    lineBuffer[count] = '\0';
    char line[count + 1];
    strncpy(s, lineBuffer, (count + 1));
    return count;
}

int cmp(unsigned char *c1, unsigned char *c2){
    for(int i = 0; i < HS; i++){
        if( *(c1 + i) != *(c2 + i)){
            return -1;
        }
    }
    return 0;
}
int main(){

    SHA256_CTX ctx;
    unsigned char hash1[HS]={236,206,10,240,218,213,228,12,63,20,93,130,211,252,50,177,182,100,85,191,151,152,181,243,245,3,5,246,127,236,44,1};
    unsigned char hash2[HS]={135,16,231,12,145,125,162,28,145,157,139,15,31,82,56,219,5,35,203,187,210,89,57,19,85,166,145,219,105,86,146,210};
    char *str;
    FILE *f1;
    unsigned char hash[HS];

    str = (char *) malloc(sizeof(char) * LEN);
    if(str == NULL){
        printf("malloc failed\n");
        return 1;
    }
    if(SHA256_Init(&ctx) == 0){
        printf("Erreur Init\n");
        return 1;
    }
    f1 = fopen("dico1.txt", "rb");
    if(f1 == NULL){
        printf("failed to open file1\n");
        return 1;
    }
    int found = 0;
    int len = read_line(f1, str);
    while(len > 0){
        
        if(SHA256((unsigned char*) str, strlen(str), hash) == 0){
            printf("Erreur Update\n");
            return 1;
        }
        if((cmp(hash, hash1) == 0)){
            printf("hash1 => %s\n", str);
            found++;
        }
        else if(cmp(hash, hash2) == 0){
            printf("hash2 => %s\n", str);
            found++;
        }
        if(found == 2){
            break;
        }
        memset(str, '0', LEN);
        len = read_line(f1, str);
    }
    free(str);
    fclose(f1);

    return 0;
}
