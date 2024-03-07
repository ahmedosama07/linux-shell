#ifndef STRINGS_H
#define STRINGS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* replace_substring(const char *string, const char *substring, const char *replacement);
char* remove_leading_spaces(char* str);
char* remove_trailing_spaces(char* str);
char* remove_quotes(char* str);

#endif /* STRINGS_H */
