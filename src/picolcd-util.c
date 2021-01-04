#include <stdio.h>
#include <string.h>
#include <ctype.h>

int is_number (const char const * string)
{
    int i, length;
    
    if (string == NULL) 
	return 0;
	
    if ((length = strlen(string)) == 0) {
	return 0;
    }
	
    for(i=0; i < length; i++) {
	if (isdigit((int)string[i]) == 0) 
	    return 0;
    }
	
    return 1;
    
}

