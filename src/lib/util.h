#ifndef UTIL_H
#define UTIL_H

void memory_copy(char* source, char* dest, int nbytes);
int string_cmp_case_sensitive(char* str1, char* str2, int len, int case_sensitive);
int string_cmp(char* str1, char* str2, int len);
void itoa(int num, char str[]);
void cth(char c, char hex[]);

#endif