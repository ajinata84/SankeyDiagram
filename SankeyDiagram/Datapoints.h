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

    
    
}

void deleteNode(SankeyLevel** levelHead, int levelInput, int nodeIndex)
{
    if (*levelHead == NULL)
    {
        printf("Invalid Level");
        return;
    }

    SankeyLevel* currLevel = *levelHead;
    SankeyLevel* prevLevel = NULL;

    while (currLevel != NULL && currLevel->level != levelInput)
    {
        prevLevel = currLevel;
        currLevel = currLevel->next;
    }

    if (currLevel == NULL)
    {
        printf("Invalid Level");
        return;
    }

    SankeyNode* currNode = currLevel->sankeyNodeHead;
    SankeyNode* prevNode = NULL;

    while (currNode != NULL && currNode->index != nodeIndex)
    {
        prevNode = currNode;
        currNode = currNode->next;
    }

    if (currNode == NULL)
    {
        printf("Invalid Node");
        return;
    }

    if (prevNode == NULL)
    {
        currLevel->sankeyNodeHead = currNode->next;
    }
    else
    {
        prevNode->next = currNode->next;
    }

    free(currNode);

    if (currLevel->sankeyNodeHead == NULL)
    {
        if (prevLevel == NULL)
        {
            *levelHead = currLevel->next;
        }
        else
        {
            prevLevel->next = currLevel->next;
        }

        free(currLevel);
    }
}

void deleteLevel(SankeyLevel** levelHead, int level) {
    SankeyLevel* currLevel = *levelHead;
    SankeyLevel* prevLevel = NULL;

    // Traverse to the specified level
    while (currLevel != NULL && currLevel->level != level) {
        prevLevel = currLevel;
        currLevel = currLevel->next;
    }

    // If level found, delete it
    if (currLevel != NULL) {
        // Update the previous level's next pointer
        if (prevLevel != NULL) {
            prevLevel->next = currLevel->next;
        }
        else {
            *levelHead = currLevel->next;
        }

        // Free memory for nodes in the level
        SankeyNode* currNode = currLevel->sankeyNodeHead;
        while (currNode != NULL) {
            SankeyNode* nextNode = currNode->next;
            free(currNode);
            currNode = nextNode;
        }

        // Free memory for the level itself
        free(currLevel);

        printf("Level %d deleted.\n", level);
    }
    else {
        printf("Level %d not found.\n", level);
    }
}



void printGraph(SankeyGraph* graphHeader) {
    SankeyGraph* currentGraph = graphHeader;
    int currLevel = 1;

    while (currentGraph != NULL) {
        printf("Level %d to %d:\n", currLevel, currLevel+1);
        printf("Size: %d, %d\n\n", currentGraph->firstEl, currentGraph->secEl);

        for (int i = 0; i < currentGraph->firstEl; i++) {
            for (int j = 0; j < currentGraph->secEl; j++) {
                printf("%d ", currentGraph->graph[i][j]);
            }
            printf("\n");
        }

        printf("\n");

        currentGraph = currentGraph->next;
        currLevel++;
    }
}