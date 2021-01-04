#include <stdio.h>
#include <string.h>

#include "picolcd.h"
#include "widgets.h"

int charmap(int value, int block)
{
	if (block * 8 >= value)
		return 30;
	if ((block + 1) * 8 <= value)
		return 255;
	if (value < (block + 1) * 8)
		return value % 8 - 1;
	if (value % 8 == 0)
		return 30;
	return value % 8 - 1;
}
void histo(usblcd_operations * mylcd, int lcdline, int lcdcol, histo_opts options, int valuecount, int values[])
{
	char matrix[64][40];
	int fontlines[64];
	int nrchars;

	int i, j, ii, jj;
	int maxvalue;
	int Xpixel = 0;
	int barwidth;
	int fontchars;

	memset(matrix, 0, sizeof(matrix));
	switch (options.orientation) {
	case 0:
	case 1:
		maxvalue = options.height * 8;
		break;
	case 2:
	case 3:
		maxvalue = options.width * 5;
		break;
	}

	Xpixel = 0;
	for (i = 0; i < valuecount; i++) {
		if (options.valuetype == 0) {
			if (maxvalue < values[i])
				values[i] = maxvalue;
		} else {
			if (values[i] > 100)
				values[i] = 100;
			values[i] = maxvalue * values[i] / 100;
		}
		for (j = 0; j < values[i]; j++) {
			for (barwidth = 0; barwidth < options.barwidth; barwidth++) {
				switch (options.orientation) {
				case 0:
					matrix[j][Xpixel + barwidth] = 1;
					break;
				case 1:
					matrix[maxvalue - j][Xpixel + barwidth] = 1;
					break;
				case 2:
					matrix[Xpixel + barwidth][j] = 1;
					break;
				case 3:
					matrix[Xpixel + barwidth][maxvalue - j] = 1;
					break;
				}
			}
		}
		Xpixel += options.barwidth;
		Xpixel += options.barspacing;
	}


	if ((options.orientation == 0) && (options.barwidth == 5)) {
		nrchars = 8;
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (8 - j <= i) {
					fontlines[i * 8 + j] = 31;
				} else {
					fontlines[i * 8 + j] = 0;
				}
			}
		}
		mylcd->setfontmemory(mylcd, fontlines, nrchars);
		for (j = 0; j < options.width; j++)
			for (i = 0; i < options.height; i++)
				mylcd->setchar(mylcd, i + lcdline, j + lcdcol,
					       charmap(values[j], 1 - i));
	} else {
		fontchars = 0;
		nrchars = 0;
		for (i = 0; i < options.height; i++) {
			for (j = 0; j < options.width; j++) {
				for (ii = 0; ii < 8; ii++) {
					fontlines[(nrchars * 8) + ii] = 0;
					for (jj = 0; jj < 5; jj++) {
						switch (options.orientation) {
						case 0:
						case 1:
							fontlines[(nrchars *  8) + ii] =
							    fontlines[(nrchars * 8) + ii] * 2 + matrix[maxvalue - (i * 8 + ii) - 1][j * 5 + jj];
							break;
						case 2:
						case 3:
							fontlines[(nrchars * 8) + ii] = fontlines[(nrchars * 8) + ii] * 2 + matrix[i * 8 + ii][j * 5 + jj];
							break;
						}
					}
				}
				fontchars++;
				nrchars++;
			}
		}

		mylcd->setfontmemory(mylcd, fontlines, nrchars);

		for (i = 0; i < options.height; i++) {
			for (j = 0; j < options.width; j++) {
				mylcd->setchar(mylcd, i + lcdline, j + lcdcol, (i * options.width) + j);
			}
		}
	}
}
