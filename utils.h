#ifndef UTILS_H
#define UTILS_H
#define BASE_STR_LEN 8
#include "game.h"

// I/O and memory
int getInt(const char* prompt);
char* getString(const char* prompt);
void *semiSafeRealloc(void *ptr, size_t newSize);
Direction getDir();
ItemType getItemType();
MonsterType getMonsterType();

// Monster functions
void freeMonster(void* data);
int compareMonsters(void* a, void* b);
void printMonster(void* data);

// Item functions
void freeItem(void* data);
int compareItems(void* a, void* b);
void printItem(void* data);




#endif
