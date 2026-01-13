#ifndef GAME_H
#define GAME_H

#define EXISTS 'V'
#define N_EXISTS 'X'

typedef enum ItemType { ARMOR, SWORD } ItemType;
typedef enum MonsterType { PHANTOM, SPIDER, DEMON, GOLEM, COBRA } MonsterType;
typedef enum Direction { UP, DOWN, LEFT, RIGHT } Direction;
typedef enum Bool { FALSE, TRUE } Bool;

#include "bst.h"

typedef struct Item {
    char* name;
    ItemType type;
    int value;
} Item;

typedef struct Monster {
    char* name;
    MonsterType type;
    int hp;
    int maxHp;
    int attack;
} Monster;

typedef struct Room {
    int id;
    int x, y;
    int visited;
    Monster* monster;
    Item* item;
    struct Room* next;
} Room;

typedef struct Player {
    int hp;
    int maxHp;
    int baseAttack;
    BST* bag;
    BST* defeatedMonsters;
    Room* currentRoom;
} Player;

typedef struct GameState {
    Room* rooms;
    Player* player;
    int roomCount;
    int configMaxHp;
    int configBaseAttack;
} GameState;

typedef struct Coordinates {
    int x;
    int y;
} Coordinates;

// Game functions
void playGame(GameState* g);
void freeGame(GameState* g);

// Memory management functions
void *safeRealloc(void *ptr, size_t newSize, GameState *g);
void *safeMalloc(size_t newSize, GameState *g);
Direction getDir();
ItemType getItemType();
MonsterType getMonsterType();

// rooms and map functions
Room *findByCoordinates(Coordinates coords, GameState *g);
Room *findByID(int id, GameState *g);
void displayMap(GameState* g);
void roomLegend(GameState* g);
void moveCoords(Coordinates *coord, Direction dir);


#endif
