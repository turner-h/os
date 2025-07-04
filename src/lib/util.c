#include "types.h"

void memory_copy(char* source, char* dest, int nbytes) {
    for (int i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void itoa(int num, char str[]){
    int i = 0;
    while (num > 0) {
        str[i] = (num % 10) + '0';
        num /= 10;
        i++;
    }
    str[i] = 0;
}

void cth(u8 c, char hex[]) {
    u8 first, second;
     
    first = c / 0x10;
    second = c % 0x10;
     
    if (first < 10) { first += '0'; }
    else {
        first -= 10;
        first += 'A';
    }
    if (second < 10)  { second += '0'; }
    else {
        second -= 10;
        second += 'A';
    }
        
    hex[0] = first;
    hex[1] = second;
    hex[2] = 0;
}