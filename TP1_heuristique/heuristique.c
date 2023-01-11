#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <openssl/sha.h>


#define HS 32
#define LEN 128
#define MAX_Ts 4

u_char hash1[HS]={99,27,154,138,53,61,181,43,178,103,27,232,132,178,4,28,133,154,85,209,110,109,164,222,54,187,31,177,253,80,245,11};
u_char hash2[HS]={229,245,177,27,190,97,124,233,8,243,83,240,14,126,250,242,18,145,85,36,202,35,92,72,237,9,28,153,123,55,253,164};
u_char hash3[HS]={14,63,208,2,84,251,207,34,204,83,177,93,182,34,91,230,59,3,181,79,78,56,0,128,234,181,221,132,219,94,40,1};
u_char hash4[HS]={106,76,233,133,75,101,137,140,40,158,113,14,229,79,187,203,104,34,90,25,238,233,219,33,17,169,126,2,127,212,70,158};
u_char hash5[HS]={204,82,170,71,151,42,43,203,171,255,86,120,21,187,141,182,250,163,78,230,231,172,14,80,113,59,87,212,133,172,76,148};
u_char hash6[HS]={77,164,88,120,143,161,91,139,204,142,176,218,67,31,210,48,45,205,25,200,202,124,14,206,21,2,78,100,70,41,225,171};
int found;

typedef int (*transf_t)(char *, char *, int);

void print_hash_hex(u_char* md, int size){

    int i;
    for(i=0;i<size;i++){
        printf("%02x ",md[i]);
    }
    printf("\n");

}

void print_hash(u_char* md, int size){

    int i;
    for(i=0;i<size;i++){
        printf("%d ",md[i]);
    }
    printf("\n");

}

int t1(char *s, char *new, int index){
    if(memset(new, '\0', LEN) == NULL){
        return -1;
    }
    if(strcpy(new, s) == NULL){
        return -1;
    }
    *new = (*new ^ 0x20);
    return 0;
}

int t2(char *s, char *new, int index){
    if(memset(new, '\0', LEN) == NULL){
        return -1;
    }
    if(strcpy(new, s) == NULL){
        return -1;
    }
    *(new + index) = (*(new + index) ^ 0x20);
    return 0;
}

int t3(char *s, char *new, int index){
    if(memset(new, '\0', LEN) == NULL){
        return -1;
    }
    if(strcpy(new, s) == NULL){
        return -1;
    }
    if(strcat(new, "1") == NULL){
        return -1;
    }
    return 0;
}

int t4(char *s, char *new, int index){
    if(memset(new, '\0', LEN) == NULL){
        return -1;
    }
    if( strcat(new, s) == NULL){
        return -1;
    }
    if( strcat(new, s) == NULL){
        return -1;
    }
    return 0;
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
    else if(cmp(hash, hash2) == 0){
        printf("hash3 => %s\n", str);
        found++;
    }
    else if(cmp(hash, hash2) == 0){
        printf("hash4 => %s\n", str);
        found++;
    }
    else if(cmp(hash, hash2) == 0){
        printf("hash5 => %s\n", str);
        found++;
    }
    else if(cmp(hash, hash2) == 0){
        printf("hash6 => %s\n", str);
        found++;
    }
    if(found == 6){
        return 0;
    }
    return -1;
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

int main(int agc, void **argv){

    transf_t t[MAX_Ts];
    char *str, *new, *new2;
    FILE *f1;
    u_char hash[HS];

    str = (char *) malloc(sizeof(char) * LEN);
    if(str == NULL){
        printf("malloc failed\n");
        return 1;
    }
    new = (char *) malloc(sizeof(char) * LEN);
    if(new == NULL){
        printf("malloc failed\n");
        return 1;
    }
    new2 = (char *) malloc(sizeof(char) * LEN);
    if(new2 == NULL){
        printf("malloc failed\n");
        return 1;
    }
    f1 = fopen("dico2.txt", "rb");
    if(f1 == NULL){
        printf("failed to open file1\n");
        return 1;
    }
    found = 0;
    t[0] = t1; t[1] = t2; t[2] = t3; t[3] = t4;
    int len = read_line(f1, str);
    while(len > 0){
        // check applying 1 t 
        for(int i = 0; i < MAX_Ts; i++){
            if(t[i](str, new, len - 1) != 0){
                printf("Error t%d\n", i + 1);
                return 1;
            }
            if(SHA256((u_char *) new, strlen(new), hash) == 0){
                printf("Erreur Update\n");
                return 1;
            }
            if(witch_hash(hash, new) == 0){
                break;
            }
        }
        // check applying 2 t
        for(int i = 0; i < MAX_Ts; i++){
            if(t[i](str, new, len - 1) != 0){
                printf("Error t%d\n", i + 1);
                return 1;
            }
            for(int j = 0; j < MAX_Ts; j++){
                if(j != i){
                    if(t[j](new, new2, strlen(new) - 1) != 0){
                        printf("Error t%d\n", i + 1);
                        return 1;
                    }

                    if(SHA256((u_char *) new2, strlen(new2), hash) == 0){
                        printf("Erreur Update\n");
                        return 1;
                    }
                    if(witch_hash(hash, new2) == 0){
                        break;
                    }
                }    
            }
        }
        if(memset(str, '\0', LEN) == NULL){
            printf("memset failed\n");
            return 1;
        }
        len = read_line(f1, str);
    }
    free(new2);
    free(new);
    free(str);
    fclose(f1);

    return 0;
}

