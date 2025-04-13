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