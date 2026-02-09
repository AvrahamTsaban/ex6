#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

/* This file implements utility functions for the game, including input handling, memory management, and operations on Monsters and Items. */


static char safeGetChar(void);

void *semiSafeRealloc(void *ptr, size_t newSize) {
// reallocate memory and exit program if allocation fails
    void *newPtr = realloc(ptr, newSize);
    if (!newPtr) {
        free(ptr);
        return NULL;
    }
    return newPtr;
}

int getInt(const char* prompt) {
    // prompt user
    printf("%s", prompt);

    // read integer from stdin with error checking
    int scan_result = 0, output = 0;
    scan_result = scanf(" %d", &output);
    while (scan_result != 1) {
        printf("Invalid input: ");
        while (getchar() != '\n') {
            continue;
        }
        scan_result = scanf(" %d", &output);
    }

    return output;
}

char *getString(const char* prompt) {
    // prompt user
    printf("%s", prompt);

    // dynamically read string from stdin until newline
    size_t currentLen = 0, capacity = BASE_STR_LEN;
    char *str = (char*)malloc((size_t)capacity * sizeof(char));
    char newChar;
    if (str == NULL) {
        return NULL;
    }
    
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
            str = (char*)semiSafeRealloc(str, capacity * sizeof(char));
            if (str == NULL) {
                return NULL;
            }
        }
        newChar = safeGetChar();
    } 

    // null-terminate str and return
    str[currentLen] = '\0';
    return str;    
}

static char safeGetChar(void) {
    // get a character from stdin, returning '\n' on EOF or null character
    int inp = getchar();
    if (inp == EOF) {
        return '\n';
    } else {
        return (char)inp;
    }
}


Direction getDir(void) {
    // get valid direction from user
    int dirInt;
    do {
        dirInt = getInt("Direction (0=Up,1=Down,2=Left,3=Right): "); 
        if (dirInt < 0 || dirInt > 3) {
            printf("Invalid direction\n");
            continue;
        }
    } while (dirInt < 0 || dirInt > 3);

    Direction dir = (Direction)dirInt;
    return dir;
}

ItemType getItemType(void) {
    // get valid item type from user
    int typeInt;
    do {
        typeInt = getInt("Type (0=Armor, 1=Sword): "); 
        if (typeInt < 0 || typeInt > 1) {
            printf("Invalid item type\n");
            continue;
        }
    } while (typeInt < 0 || typeInt > 1);

    ItemType type = (ItemType)typeInt;
    return type;
}

MonsterType getMonsterType(void) {
    // get valid monster type from user
    int typeInt;
    do {
        typeInt = getInt("Type (0-4): "); 
        if (typeInt < 0 || typeInt > 4) {
            printf("Invalid monster type\n");
            continue;
        }
    } while (typeInt < 0 || typeInt > 4);
    
    MonsterType type = (MonsterType)typeInt;
    return type;
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

    cmp = (int)itemA->type - (int)itemB->type;
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
    printf("\t[%s] Type: %s, Attack: %d, HP: %d\n", toPrint->name, typeStr, toPrint->attack, toPrint->maxHp);
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

    cmp = (int)itemA->type - (int)itemB->type;
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
            strcpy(typeStr, "ARMOR");
            break;
        case SWORD:
            strcpy(typeStr, "SWORD");
            break;
    }

    // print item details
    printf("\t[%s] %s - Value: %d\n", typeStr, toPrint->name,  toPrint->value);
}


