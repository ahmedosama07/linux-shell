#include "strings.h"

/* Function to replace all occurrences of a substring with another substring */
char* replace_substring(const char *string, const char *substring, const char *replacement) {
    int length = strlen(substring);
    const char *temp = string;
    int count = 0;
    
    // Count the number of occurrences
    while ((temp = strstr(temp, substring)) != NULL) {
        temp += length;
        count++;
    }

    // Allocate memory for the new string
    int newLength = strlen(string) + (strlen(replacement) - length) * count + 1;
    char *newString = (char*)malloc(newLength);

    if (newString == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Copy the original string and perform replacements
    temp = string;
    char *current = newString;
    while (*temp) {
        if (strstr(temp, substring) == temp) {
            strcpy(current, replacement);
            temp += length;
            current += strlen(replacement);
        } else {
            *current++ = *temp++;
        }
    }
    *current = '\0';

    return newString;
}

// Function to remove preceding white spaces from a string
char* remove_leading_spaces(char* str) {
    if (str == NULL) {
        return NULL; // Return NULL if the input string is NULL
    }
    
    int len = strlen(str);
    int i, j;

    // Find the index of the first non-space character
    for (i = 0; i < len; i++) {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r') {
            break;
        }
    }

    // Shift characters to the left to remove preceding spaces
    for (j = 0; i < len; i++, j++) {
        str[j] = str[i];
    }
    str[j] = '\0'; // Null-terminate the string at the new length

    return str;
}

// Function to remove trailing white spaces from a string
char* remove_trailing_spaces(char* str) {
    if (str == NULL) {
        return NULL; // Return NULL if the input string is NULL
    }
    
    int len = strlen(str);
    int i;

    // Find the index of the last non-space character
    for (i = len - 1; i >= 0; i--) {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r') {
            break;
        }
    }

    // Null-terminate the string at the last non-space character
    str[i + 1] = '\0';

    return str;
}


char* remove_quotes(char* str) {
    if (str == NULL) {
        return NULL; // Return NULL if the input string is NULL
    }
    
    int len = strlen(str);
    char* result = (char*)malloc((len + 1) * sizeof(char)); // Allocate memory for the result string
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }

    int j = 0;
    for (int i = 0; i < len; i++) {
        if (str[i] != '"') {
            result[j++] = str[i];
        }
    }
    result[j] = '\0'; // Null-terminate the result string

    return result;
}
