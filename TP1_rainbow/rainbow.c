#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#define HS 32




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


/**
    out: chaîne de caractères C, avec 8 caractères de mot de passe, et le 9ème qui vaut 0
    (ainsi out est conforme pour les fonctions strlen ou strcmp par exemple).
*/
void reduce(unsigned char* in, char* out){
    int i;
    for(i=0;i<8;i++){
        out[i]=(in[i]%94) + 33; //retourne le code d'un caractère imprimable entre 33 et 126 inclus
    }
}



int main()
{


    char deb1[9]="m2iliade";
    char fin1[9] = "DI^O6^]P";

    char deb2[9]="brestbmo";
    char fin2[9] ="3mb/kUsO";

    char deb3[9]="AtlaNtiC";
    char fin3[9] ="x[iHy,&+";


    unsigned char hash1[HS]={188,165,106,13,237,70,230,12,203,149,2,102,201,236,175,126,182,215,221,135,251,151,124,117,181,17,41,129,73,84,25,1};
    unsigned char hash2[HS]={88,126,235,116,250,124,132,229,105,214,230,159,147,41,160,42,218,147,6,198,178,37,138,184,16,122,99,200,212,208,105,112};
    unsigned char hash3[HS]={44,26,67,175,5,229,244,105,166,42,237,238,60,135,177,21,137,150,129,184,119,32,83,191,211,36,82,137,30,19,160,166};



    return 0;
}
