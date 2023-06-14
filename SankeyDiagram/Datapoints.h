#pragma once

#include <stdio.h>
#include <stdlib.h>

typedef struct SankeyNode
{
    int index;
    struct SankeyNode* next;
} SankeyNode;

typedef struct SankeyLevel
{
    int level;
    SankeyNode* sankeyNodeHead;
    struct SankeyLevel* next;
} SankeyLevel;

typedef struct SankeyGraph
{
    int** graph;
    int firstEl;
    int secEl;

    double* startingValues;
    double* endValues;

    double totalValue;
    struct SankeyGraph* next;
} SankeyGraph;

void insertNewNode(SankeyNode** head)
{
    SankeyNode* newNode = (SankeyNode*)malloc(sizeof(SankeyNode));

    if (!newNode)
        return;

    newNode->next = NULL;

    int index = 1;

    if (*head == NULL)
    {
        newNode->index = index;
        *head = newNode;
    }
    else
    {
        SankeyNode* current = *head;

        while (current->next != NULL)
        {
            current = current->next;
            index++;
        }
        newNode->index = index + 1;
        current->next = newNode;
    }
}

void insertNewLevel(SankeyLevel** head)
{
    SankeyLevel* levelNode = (SankeyLevel*)malloc(sizeof(SankeyLevel));

    if (!levelNode)
        return;

    levelNode->next = NULL;
    levelNode->sankeyNodeHead = NULL;

    int index = 1;

    if (*head == NULL)
    {
        levelNode->level = index;
        *head = levelNode;
    }
    else
    {
        SankeyLevel* current = *head;

        while (current->next != NULL)
        {
            current = current->next;
            index++;
        }
        levelNode->level = index + 1;
        current->next = levelNode;
    }
}

void assignNewNodeToLevel(SankeyLevel** levelHead, int levelInput)
{
    if (*levelHead == NULL)
        return;

    SankeyLevel* currHead = *levelHead;
    int flag = 0;

    if (currHead->level == levelInput)
    {
        insertNewNode(&(currHead->sankeyNodeHead));
        return;
    }

    while (currHead->next != NULL)
    {
        currHead = currHead->next;
        if (currHead->level == levelInput)
        {
            flag = 1;
            break;
        }
    }

    if (flag == 0)
    {
        printf("Invalid Level");
        return;
    }

    insertNewNode(&(currHead->sankeyNodeHead));
}

void createGraph(SankeyLevel* levelHeader, SankeyGraph** graphHeader) {
    SankeyLevel* current = levelHeader;

    while (current != NULL && current->next != NULL) {
        SankeyGraph* newGraph = (SankeyGraph*)malloc(sizeof(SankeyGraph));

        if (!newGraph) return;

        SankeyNode* currNodeHead = current->sankeyNodeHead;
        SankeyNode* nextNodeHead = (current->next != NULL) ? current->next->sankeyNodeHead : NULL;

        int firstArr = 0;
        int secondArr = 0;

        // Calculate the number of nodes in the current level
        while (currNodeHead != NULL) {
            firstArr++;
            currNodeHead = currNodeHead->next;
        }

        // Calculate the number of nodes in the next level
        while (nextNodeHead != NULL) {
            secondArr++;
            nextNodeHead = nextNodeHead->next;
        }

        // Allocate memory for the 2D array
        newGraph->firstEl = firstArr;
        newGraph->secEl = secondArr;
        newGraph->graph = (int**)malloc(sizeof(int*) * firstArr);
        newGraph->startingValues = (double*)malloc(sizeof(double) * firstArr);
        newGraph->endValues = (double*)malloc(sizeof(double) * secondArr);
        newGraph->totalValue = 0;

        if (!newGraph->graph || !newGraph->startingValues || !newGraph->endValues) return;

        for (int i = 0; i < firstArr; i++) {
            newGraph->graph[i] = (int*)malloc(sizeof(int) * secondArr);
        }

        // Initialize the graph matrix with zeros
        for (int i = 0; i < firstArr; i++) {
            for (int j = 0; j < secondArr; j++) {
                if (!newGraph->graph[i]) return;
                newGraph->graph[i][j] = 0;
            }
        }

        // Initialize startingValues array with zeros
        for (int i = 0; i < firstArr; i++) {
            newGraph->startingValues[i] = 0.0;
        }

        for (int i = 0; i < secondArr; i++) {
            newGraph->endValues[i] = 0.0;
        }

        // Add the new graph to the graph linked list
        if (*graphHeader == NULL) {
            *graphHeader = newGraph;
            newGraph->next = NULL;
        }
        else {
            SankeyGraph* currentGraph = *graphHeader;

            while (currentGraph->next != NULL) {
                currentGraph = currentGraph->next;
            }

            currentGraph->next = newGraph;
            newGraph->next = NULL;
        }

        current = current->next;
    }
}

void addLinkToGraph(SankeyGraph* graph, int start, int end, double value) {
    if (graph == NULL || graph->graph == NULL || start < 0 || end < 0 || start >= graph->firstEl || end >= graph->secEl) {
        printf("Invalid link\n");
        return;
    }

    // Set the value at the specified indices to represent the link
    graph->graph[start][end] = value;
    graph->startingValues[start] += value;
    graph->endValues[end] += value;
    graph->totalValue += value;
    
    
}

void printGraph(SankeyGraph* graphHeader) {
    SankeyGraph* currentGraph = graphHeader;

    while (currentGraph != NULL) {
        printf("Graph:\n");
        printf("Size: %d, %d\n\n", currentGraph->firstEl, currentGraph->secEl);

        for (int i = 0; i < currentGraph->firstEl; i++) {
            for (int j = 0; j < currentGraph->secEl; j++) {
                printf("%d ", currentGraph->graph[i][j]);
            }
            printf("\n");
        }

        printf("\n");

        currentGraph = currentGraph->next;
    }
}