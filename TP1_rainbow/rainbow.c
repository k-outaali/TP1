#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#define HS 32
#define LEN 128


unsigned char hash1[HS]={188,165,106,13,237,70,230,12,203,149,2,102,201,236,175,126,182,215,221,135,251,151,124,117,181,17,41,129,73,84,25,1};
unsigned char hash2[HS]={88,126,235,116,250,124,132,229,105,214,230,159,147,41,160,42,218,147,6,198,178,37,138,184,16,122,99,200,212,208,105,112};
unsigned char hash3[HS]={44,26,67,175,5,229,244,105,166,42,237,238,60,135,177,21,137,150,129,184,119,32,83,191,211,36,82,137,30,19,160,166};

int found = 0;

void clean(char* inout, int s){
    int i;
    for(i=0;i<s;i++){
        inout[i]=0;
    }

}

void affiche_hashHex(unsigned char* md, int size){

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

void reduce(unsigned char* in, char* out){
    int i;
    for(i=0;i<8;i++){
        out[i]=(in[i]%94) + 33; //returns printable chars between 33 et 126 included
    }
}

int cmp(u_char *c1, u_char *c2){
    for(int i = 0; i < HS; i++){
        if( *(c1 + i) != *(c2 + i)){
            return -1;
        }
    }
    return 0;
}

int witch_hash(u_char *hash, char *str){
    if((cmp(hash, hash1) == 0)){
        printf("hash1 => %s\n", str);
        found++;
    }
    else if(cmp(hash, hash2) == 0){
        printf("hash2 => %s\n", str);
        found++;
    }
    else if(cmp(hash, hash3) == 0){
        printf("hash3 => %s\n", str);
        found++;
    }
    if(found == 3){
        return 0;
    }
    return -1;
}


int main(){

    char deb1[9]="m2iliade";
    char fin1[9] = "DI^O6^]P";

    char deb2[9]="brestbmo";
    char fin2[9] ="3mb/kUsO";

    char deb3[9]="AtlaNtiC";
    char fin3[9] ="x[iHy,&+";

    u_char hash[HS];
 
    char *cur = malloc(LEN);
    strcpy(cur, deb1);
    while(strcmp(cur, fin1) != 0){
        SHA256((u_char *) cur, strlen(cur), hash);
        if(witch_hash(hash, cur) == 0){
            break;
        }
        memset(cur, '\0', LEN);
        reduce(hash, cur);
    }

    strcpy(cur, deb2);
    while(strcmp(cur, fin2) != 0){
        SHA256((u_char *) cur, strlen(cur), hash);
        if(witch_hash(hash, cur) == 0){
            break;
        }
        memset(cur, '\0', LEN);
        reduce(hash, cur);
    }

    strcpy(cur, deb3);
    while(strcmp(cur, fin3) != 0){
        SHA256((u_char *) cur, strlen(cur), hash);
        if(witch_hash(hash, cur) == 0){
            break;
        }
        memset(cur, '\0', LEN);
        reduce(hash, cur);
    }

    free(cur);
    return 0;
}
