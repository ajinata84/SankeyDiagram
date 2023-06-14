#include <stdio.h>
#include <cairo/cairo.h>

#include "Datapoints.h"
#include "DrawSankey.h"

#define MAX_HEIGHT 1000
#define MAX_WIDTH 1400

int main() {
	cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, MAX_WIDTH, MAX_HEIGHT);
	cairo_t* cr = cairo_create(surface);

	SankeyNode* head = NULL;
	SankeyLevel* levelHead = NULL;
	SankeyGraph* graphHead = NULL;
	int totalLevels = 3;

	insertNewLevel(&levelHead);
	insertNewLevel(&levelHead);
	insertNewLevel(&levelHead);

	assignNewNodeToLevel(&levelHead, 1);
	assignNewNodeToLevel(&levelHead, 1);
	assignNewNodeToLevel(&levelHead, 1);
	assignNewNodeToLevel(&levelHead, 2);
	assignNewNodeToLevel(&levelHead, 2);
	assignNewNodeToLevel(&levelHead, 2);
	assignNewNodeToLevel(&levelHead, 2);
	assignNewNodeToLevel(&levelHead, 2);
	assignNewNodeToLevel(&levelHead, 3);
	assignNewNodeToLevel(&levelHead, 3);
	assignNewNodeToLevel(&levelHead, 3);
	assignNewNodeToLevel(&levelHead, 3);

	createGraph(levelHead, &graphHead);

	addLinkToGraph(graphHead, 0, 0, 4);
	//addLinkToGraph(graphHead, 0, 1, 6);
	//addLinkToGraph(graphHead, 0, 4, 2);
	addLinkToGraph(graphHead, 1, 3, 8);
	addLinkToGraph(graphHead, 1, 2, 5);
	addLinkToGraph(graphHead, 1, 4, 4);
	addLinkToGraph(graphHead, 2, 1, 7);
	addLinkToGraph(graphHead, 2, 3, 9);
	addLinkToGraph(graphHead, 2, 0, 3);
	addLinkToGraph(graphHead, 2, 4, 8);

	addLinkToGraph(graphHead->next, 0, 3,7);
	addLinkToGraph(graphHead->next, 1, 3,3);
	addLinkToGraph(graphHead->next, 1, 2,4);
	addLinkToGraph(graphHead->next, 2, 1,3);
	addLinkToGraph(graphHead->next, 2, 3,1);
	addLinkToGraph(graphHead->next, 2, 0,1);
	addLinkToGraph(graphHead->next, 3, 3,17);
	addLinkToGraph(graphHead->next, 4, 3, 12);

	drawLines(cr, graphHead, totalLevels, MAX_HEIGHT, MAX_WIDTH);

	// Save the diagram to an image file
	cairo_surface_write_to_png(surface, "sankey_diagram.png");

	// Clean up resources
	cairo_destroy(cr);
	cairo_surface_destroy(surface);

	return 0;
}