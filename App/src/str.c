#include <str.h>

//gets the length of a string
uint64_t strlen(char* str){
    uint64_t len = 0;

    while(str[len] != '\0'){
        len++;
    }

    return len;
}
//gets the length of a wide character
uint64_t wstrlen(wchar_t* str){
    uint64_t len = 0;

    while(str[len] != L'\0'){
        len++;
    }

    return len;
}

//mallocs a new string which is the concatenation of the two strings
char* StrAppend(char* a, char* b){
    char* newstr = malloc(strlen(a)+strlen(b)+1);
    uint64_t vel = 0;

    while(*a != '\0'){
        newstr[vel] = *a;
        vel++;
        a++;
    }
    while(*b != '\0'){
        newstr[vel] = *b;
        vel++;
        b++;
    }
    newstr[vel] = '\0';

    return newstr;
}



//converts a char string to wchar_t string
void charstr_to_wcharstr(char* src, wchar_t* dst){
    uint64_t i = 0;

    while(src[i] != '\0'){
        dst[i] = (wchar_t)src[i];
        i++;
    }
    //add null terminator
    dst[i] = L'\0';
}

//converts a wchar_t string to char string
void wcharstr_to_charstr(wchar_t* src, char* dst){
    uint64_t i = 0;

    while(src[i] != L'\0'){
        dst[i] = (char)src[i];
        i++;
    }
    //add null terminator
    dst[i] = '\0';
}



