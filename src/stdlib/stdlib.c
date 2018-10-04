#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

static unsigned long int RAND_NEXT = 1;

int32_t rand(void)
{
    RAND_NEXT = RAND_NEXT * 1103515245 + 12345;
    return (uint32_t)(RAND_NEXT/65536) % 32768;
}

void srand(uint32_t seed)
{
    RAND_NEXT = seed;
}

void reverse(char s[])
{
    int i;
    size_t j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

char* itoa(int num, char* str, int base) 
{ 
    int i = 0; 
    bool isNegative = false; 
  
    // Handle 0 explicitely
    if (num == 0) 
    { 
        str[i++] = '0'; 
        str[i] = '\0'; 
        return str; 
    } 
  
    // In standard itoa(), negative numbers are handled only with  
    // base 10. Otherwise numbers are considered unsigned. 
    if (num < 0 && base == 10) 
    { 
        isNegative = true; 
        num = -num; 
    } 
  
    // Process individual digits 
    while (num != 0) 
    { 
        int rem = num % base; 
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0'; 
        num = num/base; 
    } 
  
    // If number is negative, append '-' 
    if (isNegative){
        str[i++] = '-';
    }
  
    str[i] = '\0'; // Append string terminator 
  
    // Reverse the string 
    reverse(str); 
  
    return str; 
}

int atoi(const char* str) 
{ 
    int res = 0;
   
    // Iterate through all characters of input string 
    for (int i = 0; str[i] != '\0'; ++i){
        res = res*10 + str[i] - '0';
    }

    return res; 
}

long int atol(const char* str)
{
    long int result = 0;
    uint32_t digit;
    int32_t sign;

    while (isspace(*str)) {
	    str += 1;
    }

    if (*str == '-') {
        sign = 1;
        str += 1;
    } else {
	    sign = 0;
        if (*str == '+') {
            str += 1;
        }
    }

    for ( ; ; str += 1) {
	    digit = *str - '0';
        if (digit > 9) {
            break;
        }
	    result = (10*result) + digit;
    }

    if (sign) {
	    return -result;
    }

    return result;
}