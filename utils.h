#ifndef UTILS_H
#define UTILS_H
#define BASE_STR_LEN 8


// I/O and memory
int getInt(const char* prompt);
char* getString(const char* prompt, GameState *g);
void *safeRealloc(void *ptr, size_t newSize, GameState *g);
void *safeMalloc(size_t newSize, GameState *g);
Direction getDir ();

// Monster functions
void freeMonster(void* data);
int compareMonsters(void* a, void* b);
void printMonster(void* data);

// Item functions
void freeItem(void* data);
int compareItems(void* a, void* b);
void printItem(void* data);

// rooms and map functions
Room *findByCoordinates(Coordinates coords, GameState *g);
Room *findByID(int id, GameState *g);
void displayMap(GameState* g);
void roomLegend(GameState* g);
void moveCoords(Coordinates *coord, Direction dir);


#endif
