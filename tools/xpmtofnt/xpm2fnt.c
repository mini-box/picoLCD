/* Copyright 2006 iTuner Corporation */
/* npavel@ituner.com */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mini-box-logo.xpm"

int main(void)
{
    int i,j,k;
    int indexcol = 1;
    char *fontline;
    FILE *fp;
    char filename[50] = "minibox.";
    
    fontline = (char *) malloc(6);
    for (k = 0; k < 20; k++) {
	if (indexcol == 9 || k==0 ) {
	    indexcol = 1;
	    fp = fopen(strcat(filename,"1"),"w");
	    fprintf(fp,":font\n\n");
	}
	fprintf(fp,"\n%02d\n",indexcol++);
    for (j = 3; j <= 17; j++) {
	if ((j-3)%8 == 0 && j != 3) {
	    if (indexcol == 9) indexcol = 1;
	    fprintf(fp,"\n%02d\n", indexcol++);
	}
	for (i = 0; i < 5; i++) {
	    if (mini_box_logo_xpm[j][i+k*5] == ' ')   fontline[i] = 1 + '0';
	    else fontline[i] = 0 + '0';
	}
	fprintf(fp,"%s\n", fontline);
    }
    }
    return 0;
}
