#pragma once

#include <cairo/cairo.h>
#include <time.h>

#include "DataPoints.h"

double calcRGB(double color) {
	return color / 255;
}

double getEvenSpaces(int size, int index, int total) {
	return (index + 1) * (size / (total + 1));
}

void randomizeColor(double* red, double* green, double* blue) {
	*red = (double)rand() / RAND_MAX;
	*green = (double)rand() / RAND_MAX;
	*blue = (double)rand() / RAND_MAX;
}

void drawLines(cairo_t* cr, SankeyGraph* grapHeader, int totalLevels, int height, int width) {
	SankeyGraph* currGraph = grapHeader;

	cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 13);

	int currLv = 0;
	while (currGraph != NULL) {

		double currXpos = getEvenSpaces(width, currLv, totalLevels);
		double nextXpos = getEvenSpaces(width, currLv + 1, totalLevels);

		double controlPoint = (currXpos + nextXpos) / 2;

		double totalValue = currGraph->totalValue;

		double* prevEndValue = (double*)malloc(sizeof(double) * currGraph->secEl);

		if (!prevEndValue) return;

		for (int n = 0; n < currGraph->secEl; n++)
		{
			prevEndValue[n] = 0;
		}

		for (int i = 0; i < currGraph->firstEl; i++) {
			double red, green, blue;
			double fYpos = getEvenSpaces(height, i, currGraph->firstEl);
			double currMaxValue = currGraph->startingValues[i];

			/*cairo_move_to(cr, currXpos, fYpos - currMaxValue);
			cairo_set_source_rgb(cr, 1, 1, 1);
			cairo_show_text(cr, "ajajaja");*/

			randomizeColor(&red, &green, &blue);
			double prevValue = 0;

			for (int j = 0; j < currGraph->secEl; j++) {
				if (currGraph->graph[i][j] > 0) {
					double endMaxValue = currGraph->endValues[j];

					double currValue = currGraph->graph[i][j];

					double sYpos = getEvenSpaces(height, j, currGraph->secEl);

					double startingYPos = fYpos - currMaxValue/2;
					double endYpos = sYpos - endMaxValue / 2;

					cairo_set_line_width(cr, currValue);

					double drawingYpos = (startingYPos + currValue / 2) + prevValue;
					double endDrawYpos = (endYpos + currValue / 2) + prevEndValue[j];
						
					//printf("endval %.2lf\n", endDrawYpos);
					cairo_move_to(cr, currXpos, drawingYpos);
					
					cairo_set_source_rgba(cr, red, green, blue, 0.5);
					cairo_curve_to(cr, controlPoint, drawingYpos, controlPoint, endDrawYpos, nextXpos, endDrawYpos);

					cairo_stroke(cr);

					prevValue += currValue;
					prevEndValue[j] += currValue;
				}
			}
		}
		currLv++;
		currGraph = currGraph->next;
	}
}