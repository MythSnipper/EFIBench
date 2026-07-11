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

uint64_t StrCopy(char* src, char* dst){
    uint64_t copied = 0;
    while(*src != '\0'){
        *dst = *src;
        copied++;
        src++;
        dst++;
    }
    *dst = '\0';
    return copied;
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


void sprint2ch(char* buf, uint64_t num){
    buf[0] = (char)((num%100) / 10 + '0');
    buf[1] = (char)(num%10 + '0');
}

void sprint4ch(char* buf, uint64_t num){
    buf[0] = (char)((num%10000) / 1000 + '0');
    buf[1] = (char)((num%1000) / 100 + '0');
    buf[2] = (char)((num%100) / 10 + '0');
    buf[3] = (char)(num%10 + '0');
}

void sprint_fixed_double(char* buf, double num){
    uint64_t scaled = (uint64_t)(num * SPRINT_FIXED_SCALE + 0.5); //add 0.5 so trucation means rounding to nearest

    for(uint64_t i=0;i<SPRINT_FIXED_WIDTH;i++){
        buf[SPRINT_FIXED_WIDTH-1-i] = (char)(scaled%10 + '0');
        scaled /= 10;
    }
}
void sprint_fixed_int(char* buf, uint64_t num){
    uint64_t scaled = num * SPRINT_FIXED_SCALE;

    for(uint64_t i=0;i<SPRINT_FIXED_WIDTH;i++){
        buf[SPRINT_FIXED_WIDTH-1-i] = (char)(scaled%10 + '0');
        scaled /= 10;
    }    
}
double parse_fixed_double(char* buf){
    uint64_t ret = 0.0;
    for(uint64_t i=SPRINT_FIXED_WIDTH-1;i>=0;i--){
        ret *= 10;
        ret += (buf[i] - '0');
    }
    return ((double)(ret) / SPRINT_FIXED_SCALE);
}








