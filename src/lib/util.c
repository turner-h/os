#include "types.h"
#include "../drivers/vga.h"

void memory_copy(char* source, char* dest, int nbytes) {
    for (int i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

int string_cmp_case_sensitive(char* str1, char* str2, int len, int case_sensitive) {
    for (int i = 0; i < len; i++) {
        u8 c1 = *(str1 + i);
        u8 c2 = *(str2 + i);
        
        if (!case_sensitive) {
            if (c1 > 0x5A) {
                c1 -= 0x20;
            }

            if (c2 > 0x5A) {
                c2 -= 0x20;
            }
        }


        if(c1 != c2) {
            return 0;
        }
    }

    return 1;
}

int string_cmp(char* str1, char* str2, int len) {
    return string_cmp_case_sensitive(str1, str2, len, 0);
}

void itoa(int num, char str[]){
    char temp[4];
    
    if(num == 0) {
        str[0] = '0';
        str[1] = 0;
        return;
    }

    int i = 0;
    while (num > 0) {
        temp[i] = (num % 10) + '0';
        num /= 10;
        i++;
    }

    int j = i;
    while (j > 0) {
        str[j - 1] = temp[i - j];
        j--;
    }

    str[i] = 0;
}

void cth(char c, char hex[]) {
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