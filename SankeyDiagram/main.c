#include <stdio.h>
#include <stdlib.h>
#include <cairo/cairo.h>

#include "Datapoints.h"
#include "DrawSankey.h"

#define MAX_HEIGHT 1000
#define MAX_WIDTH 1400

int totalLevels = 0;

void printMenu(SankeyLevel** levelHeader) {

	SankeyLevel* levelHead = levelHeader;
	int lv = 0;
	printf("Node Viewer\n\n");
	if (levelHead != NULL && levelHead->sankeyNodeHead != NULL) {
		SankeyLevel* currLevel = levelHead;
		while (currLevel != NULL) {
			lv++;
			SankeyNode* currList = currLevel->sankeyNodeHead;
			printf("Level %d\n", lv);

			while (currList != NULL) {
				printf("%d->", currList->index);
				currList = currList->next;
			}
			printf("\n\n");
			currLevel = currLevel->next;
		}
	}

	printf("----- Menu -----\n");
	printf("1. Create new node\n");
	printf("2. Create new level\n");
	printf("3. Delete Node\n");
	printf("4. Delete Entire Level\n");
	printf("5. Proceed to graph editor\n");
	printf("0. Exit\n");
	printf("----------------\n");
	printf("Enter your choice: ");
}

int getUserChoice(int min, int max) {
	int choice;
	printf("Enter your choice: ");
	scanf("%d", &choice);
	while (choice < min || choice > max) {
		printf("Invalid choice. Please enter a valid choice: ");
		scanf("%d", &choice);
	}
	return choice;
}

void createNewNode(SankeyLevel** levelHead) {
	int level;
	printf("Enter the level to assign a new node to: ");
	scanf("%d", &level);

	assignNewNodeToLevel(levelHead, level);
	printf("New node assigned to level %d.\n", level);
}

void createNewLevel(SankeyLevel** levelHead) {
	insertNewLevel(levelHead);
	printf("New level created.\n");
}

void addEditGraphValues(SankeyGraph* graphHead) {
    int level;
    printf("Enter the level of the graph to start from: ");
    scanf("%d", &level);

    int start, end;
    printf("Enter the starting node index: ");
    scanf("%d", &start);
    printf("Enter the ending node index: ");
    scanf("%d", &end);

    int value;
    printf("Enter the value of the link: ");
    scanf("%d", &value);

	SankeyGraph* graph = graphHead;

	int currLevel = 1;

	while (graph != NULL && currLevel < level) {
		graph = graph->next;
		currLevel++;
	}

	start -= 1;
	end -= 1;
	if (graph == NULL || graph->graph == NULL || start < 0 || end < 0 || start >= graph->firstEl || end >= graph->secEl) {
		printf("Invalid link\n");
		return;
	}

	graph->graph[start][end] = value;
	graph->startingValues[start] += value;
	graph->endValues[end] += value;
	graph->totalValue += value;

    printf("Graph values updated.\n");
	printf("%d", value);
}


void deleteNodeMenu(SankeyLevel** levelHead) {
	int level, nodeIndex;

	printf("Enter the level of the graph to delete: ");
	scanf("%d", &level);

	printf("Enter the deleted node index: ");
	scanf("%d", &nodeIndex);
	deleteNode(levelHead, level, nodeIndex);
}

void deleteLevelMenu(SankeyLevel** levelHead) {
	int level;

	printf("Enter the level to delete: ");
	scanf("%d", &level);

	deleteLevel(levelHead, level);
}

void printGraphEditorMenu() {
	printf("----- Graph Editor -----\n");
	printf("1. Edit Values\n");
	printf("2. Output Graph\n");
	printf("0. Back to Node Editor\n");
	printf("----------------\n");
	printf("Enter your choice: ");
}

void graphEditor(SankeyLevel* levelHead) {

	cairo_surface_t* surface;
	cairo_t* cr;
	
	SankeyGraph* graphHead = NULL;

	createGraph(levelHead, &graphHead);

	int choice;
	do {
		system("cls");
		printGraph(graphHead);
		printGraphEditorMenu();

		choice = getUserChoice(0, 2); 
		switch (choice) {
		case 1:
			addEditGraphValues(graphHead);
			break;
		case 2:
			surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, MAX_WIDTH, MAX_HEIGHT);
			cr = cairo_create(surface);
			drawLines(cr, graphHead, totalLevels, MAX_HEIGHT, MAX_WIDTH);
			cairo_surface_write_to_png(surface, "sankey_diagram.png");
			cairo_destroy(cr);
			cairo_surface_destroy(surface);
			printf("Success");
			break;
		case 0:
			printf("Exiting graph editor...\n");
			break;
		default:
			printf("Invalid choice. Please try again.\n");
			break;
		}
	} while (choice != 0);
}



int main() {
	SankeyLevel* levelHead = NULL;

	int choice;
	do {
		system("cls");
		printMenu(levelHead);
		choice = getUserChoice(0, 5);

		switch (choice) {
		case 1:
			createNewNode(&levelHead);
			break;
		case 2:
			createNewLevel(&levelHead);
			totalLevels++;
			break;
		case 3:
			deleteNodeMenu(&levelHead);
			break;
		case 4:
			deleteLevelMenu(&levelHead);
			totalLevels--;
			break;
		case 5:
			graphEditor(levelHead);
			break;
		case 0:
			printf("Exiting...\n");
			break;
		default:
			printf("Invalid choice. Please try again.\n");
			break;
		}

		printf("\n");
	} while (choice != 0);
	return 0;
}
