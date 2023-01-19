// just playing with variadic functions and signals
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#define vf(M, ...) printf(M , ##__VA_ARGS__);

void generic_vf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format != '\0') {
        if (*format == 'd') {
            int i = va_arg(args, int);
            printf("%d\n", i);
        } else if (*format == 'f') {
            double d = va_arg(args, double);
            printf("%f\n", d);
        } else if (*format == 's') {
            char *s = va_arg(args, char *);
            printf("%s\n", s);
        }
        ++format;
    }

    va_end(args);
}

void handler(int sig){
    printf("\nrecieved a signal = %d\n", sig);
    vf("hello %d world\n", 999);
    generic_vf("dfs", 1, 2.5, "hello");
    exit(1);
}

int main(){

    signal(SIGINT, handler);
    while(1);
    return 0;
}