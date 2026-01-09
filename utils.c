#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "game.h"


void *safeRealloc(void *ptr, size_t newSize, GameState* g) {
// reallocate memory and exit program if allocation fails (frees ptr if newSize is 0)
    if (newSize == 0) {
        // making some undefined compiler behaviour well-defined
        free(ptr);
        return ptr;
    }

    void *newPtr = realloc(ptr, newSize);
    if (!newPtr) {
        free(ptr);
        freeGame(g);
        exit(1);
    }

    return newPtr;
}

void *safeMalloc(size_t newSize, GameState* g) {
// allocate memory and exit program if allocation fails
    void *ptr = malloc(newSize);
    if (ptr == NULL) {
        freeGame(g);
        exit(1);
    }
    return ptr;
}

int getInt(const char* prompt) {
    // prompt user
    printf("%s", prompt);

    // read integer from stdin with error checking
    int scan_result = 0, output = 0;
    while (scan_result != 1) {
        scan_result = scanf(" %d", &output);
    }
    while (getchar() != '\n') {
        continue;
    }

    return output;
}

char *getString(const char* prompt, GameState *g) {
    // prompt user
    printf("%s", prompt);

    // dynamically read string from stdin until newline
    size_t currentLen = 0, capacity = BASE_STR_LEN;
    char *str = safeMalloc(capacity * sizeof(char), g), newChar ;
    
    // skip leading newlines if any
    do {
        newChar = safeGetChar();
    } while (newChar == '\n' || newChar == '\0');
    
    // read characters until newline and append to str
    while (newChar != '\n' && newChar != '\0') { 
        // append newChar to str
        str[currentLen] = newChar;
        ++currentLen;
        // check if need to expand str
        if (currentLen >= capacity) {
            capacity *= 2;
            str = safeRealloc(str, capacity * sizeof(char), g);
        }
        newChar = safeGetChar();
    } 

    // null-terminate str and return
    str[currentLen] = '\0';
    return str;    
}

static char safeGetChar() {
    // get a character from stdin, returning '\n' on EOF or null character
    int inp = getchar();
    if (inp == EOF) {
        return '\n';
    } else {
        return (char)inp;
    }
}

/**********
**Monster**
*and*Item**
**actions**
**********/

void freeMonster(void* data) {
    // check for null pointer
    if (data == NULL) {
        return;
    }
    
    // cast data to Monster pointer
    Monster *toFree = (Monster*)data;

    // free monster struct and its name
    if (toFree->name != NULL) {
        free(toFree->name);
    }
    toFree->name = NULL;
    free(toFree);
}

int compareMonsters(void* ptrA, void* ptrB) {
    // check for null pointers
    if (ptrA == NULL || ptrB == NULL) {
        return 0;
    }
    
    // cast pointers to Monster pointers
    Monster *itemA = (Monster*)ptrA;
    Monster *itemB = (Monster*)ptrB;
    
    // check for null names
    if (itemA->name == NULL || itemB->name == NULL) {
        return 0;
    }
    
    // compare two monsters by name, attack, maxHp, type (in that order)
    int cmp;

    cmp = strcmp(itemA->name, itemB->name);
    if (cmp != 0) {
        return cmp;
    }

    cmp = itemA->attack - itemB->attack;
    if (cmp != 0) {
        return cmp;
    }

    cmp = itemA->maxHp - itemB->maxHp;
    if (cmp != 0) {
        return cmp;
    }

    cmp = itemA->type - itemB->type;
    return cmp;
}

void printMonster(void* data) {
    // check for null pointer
    if (data == NULL) {
        return;
    }
    
    // cast data to Monster pointer
    Monster *toPrint = (Monster*)data;

    // check for null details
    if (toPrint->name == NULL) {
        return;
    }

    // cast monster type to string
    char typeStr[BASE_STR_LEN];
    switch (toPrint->type) {
    case PHANTOM:
        strcpy(typeStr, "Phantom");
        break;
    case SPIDER:
        strcpy(typeStr, "Spider");
        break;
    case DEMON:
        strcpy(typeStr, "Demon");
        break;
    case GOLEM:
        strcpy(typeStr, "Golem");
        break;
    case COBRA:
        strcpy(typeStr, "Cobra");
        break;
    }

    // print monster details
    printf("[%s] Type: %s, Attack: %d, HP: %d\n", toPrint->name, typeStr, toPrint->attack, toPrint->maxHp);
}

void freeItem(void* data) {
    // check for null pointer
    if (data == NULL) {
        return;
    }
    
    // cast data to Item pointer and free its name and struct
    Item *toFree = (Item*)data;
    if (toFree->name != NULL) {
        free(toFree->name);
    }
    toFree->name = NULL;
    free(toFree);
}

int compareItems(void* ptrA, void* ptrB) {
    // check for null pointers
    if (ptrA == NULL || ptrB == NULL) {
        return 0;
    }
    
    // cast pointers to Item pointers
    Item *itemA = (Item*)ptrA;
    Item *itemB = (Item*)ptrB;

    // check for null names
    if (itemA->name == NULL || itemB->name == NULL) {
        return 0;
    }

    // compare two items by name, value, type (in that order)
    int cmp;

    cmp = strcmp(itemA->name, itemB->name);
    if (cmp != 0) {
        return cmp;
    }

    cmp = itemA->value - itemB->value;
    if (cmp != 0) {
        return cmp;
    }

    cmp = itemA->type - itemB->type;
    return cmp;
}

void printItem(void* data) {
    // check for null pointer
    if (data == NULL) {
        return;
    }
    
    // cast data to Item pointer
    Item *toPrint = (Item*)data;

    // check for null name
    if (toPrint->name == NULL) {
        return;
    }

    // cast item type to string
    char typeStr[BASE_STR_LEN];
    switch (toPrint->type) {
        case ARMOR:
            strcpy(typeStr, "Armor");
            break;
        case SWORD:
            strcpy(typeStr, "Sword");
            break;
    }

    // print item details
    printf("[%s] %s - Value: %d\n", typeStr, toPrint->name,  toPrint->value);
}

/**********
***Rooms***
**search***
**********/

Room *findByID(int id, GameState *g) {
    // check for null game state or empty rooms list
    if (g == NULL || g->rooms == NULL) {
        return NULL;
    }

    // search for room with given id in game state g
    Room *iter = g->rooms;
    while (iter->next != NULL) {
        if (iter->id == id) {
            return iter;
        }
        iter = iter->next;
    }

    return NULL;
}

int isOccupied(Room *newRoom, GameState *g) {
    // check for null game state or empty rooms list
    if (g == NULL || g->rooms == NULL) {
        return NULL;
    }

    // search for room with same coordinates as newRoom in game state g
    Room *iter = g->rooms;
    while (iter->next != NULL) {
        if (iter->x == newRoom->x && iter->y == newRoom->y) {
            return 1;
        }
        iter = iter->next;
    }
    
    return 0; 
}

/**********
****Map****
**Helpers**
**********/

void displayMap(GameState* g) {
    // check for empty rooms list
    if (!g->rooms) return;
    
    // Find bounds
    int minX = 0, maxX = 0, minY = 0, maxY = 0;
    for (Room* r = g->rooms; r; r = r->next) {
        if (r->x < minX) minX = r->x;
        if (r->x > maxX) maxX = r->x;
        if (r->y < minY) minY = r->y;
        if (r->y > maxY) maxY = r->y;
    }
    
    int width = maxX - minX + 1;
    int height = maxY - minY + 1;
    
    // Create grid
    int** grid = malloc(height * sizeof(int*));
    for (int i = 0; i < height; i++) {
        grid[i] = malloc(width * sizeof(int));
        for (int j = 0; j < width; j++) grid[i][j] = -1;
    }
    
    // fill grid with room ids
    for (Room* r = g->rooms; r; r = r->next)
        grid[r->y - minY][r->x - minX] = r->id;
    
    // print grid
    printf("=== SPATIAL MAP ===\n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] != -1) printf("[%2d]", grid[i][j]);
            else printf("    ");
        }
        printf("\n");
    }
    
    // free grid
    for (int i = 0; i < height; i++) free(grid[i]);
    free(grid);
}

void roomLegend(GameState* g) {
    // check for empty rooms list
    if (!g->rooms) return;

    // print legend for rooms in game state g
    printf("=== ROOM LEGEND ===\n");

    int i = 0;
    for (Room *iter = g->rooms; iter != NULL; iter = iter->next) {
        printf("ID %d: [M:%c] [I:%c]", ++i, iter->monster ? EXISTS : N_EXISTS, iter->item ? EXISTS : N_EXISTS);
    }

    printf("===================\n");

}