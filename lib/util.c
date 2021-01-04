#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "util.h"

unsigned int bin2int (char *string) 
{
    int len;
    int i;
    int n = 0;
    
    len = strlen(string);
    if (len == 0) return n;
    
    for (i = 0; i < len; i++) 
	if ((string[i] - '0') > 1 || (string[i] - '0') < 0) return n;
	else 
	    n = n * 2 +  ( string[i] - '0');

    return n;
}


void print_buffer(char *bytes, int len) 
{
    int i;
    if (len > 0) {
	for (i=0; i<len; i++) {
	    fprintf(stderr,"%02x ", (int)((unsigned char)bytes[i]));
	}
	fprintf(stderr,"\"");
        for (i=0; i<len; i++) {
	    fprintf(stderr,"%c", isprint(bytes[i]) ? bytes[i] : '.');
        }
        fprintf(stderr,"\"\n");
    }
}
