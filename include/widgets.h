#ifndef __WIDGETS_H__
#define __WIDGETS_H__

typedef struct _histo_opts_s histo_opts;
struct _histo_opts_s {
	int width, height;	// dimension in chars of the drawing zone
	int orientation;	// 0 = bottom to top, 1 = top to bottom, 2 = left to rigth, 3 = right to left
	int valuetype;		// 0 = absolute values, 1 = percentage of maximum
	int barwidth, barspacing;
};


void histo(usblcd_operations * mylcd, int lcdline, int lcdcol, histo_opts options, int valuecount, int values[]);

#endif //__WIDGETS_H__

