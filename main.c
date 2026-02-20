#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include "game.h"
#include "utils.h"

typedef void (*ActionFunc)(GameState*);

static void addRoom(GameState* g);
static void initPlayer(GameState* g);
static Monster *getMonster(GameState *g);
static Item *getItem(GameState *g);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <player_hp> <base_attack>\n", argv[0]);
        return 1;
    }

    GameState game = {0};
    game.configMaxHp = atoi(argv[1]);
    game.configBaseAttack = atoi(argv[2]);
    ActionFunc actions[] = {NULL, addRoom, initPlayer, playGame};

    int running = 1;
    while (running) {
        printf("\n=== MENU ===\n1.Add Room\n2.Init Player\n3.Play\n4.Exit\n");
        int c = getInt("Choice: ");
        if (c == 4) running = 0;
        else if (c >= 1 && c <= 3) actions[c](&game);
    }

    freeGame(&game);
    return 0;
}

static void addRoom(GameState* g) {
    // check for null game state
    if (g == NULL) {
        return;
    }

    // create new room and zero irrelevant fields
    Room *newRoom = (Room*)safeMalloc(sizeof(Room), g);
    newRoom->visited = 0;

    // set room coordinates and ID
    if (g->roomCount == 0) {
        newRoom->x = 0;
        newRoom->y = 0;
        newRoom->id = 0;
        ++(g->roomCount);
    } else {
        // display map and legend for user to choose attachment point
        displayMap(g);
        roomLegend(g);

        // get anchor room ID and direction to place new room
        int attachTo = getInt("Attach to room ID: ");
        Room *anchor = findByID(attachTo, g);
        if (anchor == NULL) {
            printf("Invalid ID\n");
            free(newRoom);
            return;
        }

        Direction direction = getDir();
        Coordinates coords = {anchor->x, anchor->y};
        moveCoords(&coords, direction);

        // check if room already exists at new coordinates or moveCoords failed
        if (findByCoordinates(coords, g) != NULL || (coords.x == anchor->x && coords.y == anchor->y)) {
            printf("Room exists there\n");
            free(newRoom);
            return;
        }

        newRoom->x = coords.x;
        newRoom->y = coords.y;
        newRoom->id = g->roomCount;
        ++(g->roomCount);
    }

    // set monster and item for new room
    newRoom->monster = getMonster(g);
    newRoom->item = getItem(g);

    // add new room to game state's rooms list
    if (g->rooms == NULL) {
        g->rooms = newRoom;
        newRoom->next = NULL;
    } else {
        newRoom->next = g->rooms;
        g->rooms = newRoom;
    }

    // confirm room creation
    printf("Created room %d at (%d,%d)\n", newRoom->id, newRoom->x, newRoom->y);
}

static Monster *getMonster(GameState *g) {
    // ask user if they want to add a monster
    int hasMonster = getInt("Add monster? (1=Yes, 0=No): ");
    Monster *monster = NULL;

    // if yes, create new monster and get its details from user
    if (hasMonster) {
        monster = (Monster*)safeMalloc(sizeof(Monster), g);
        monster->name = getString("Monster name: ");
        if (monster->name == NULL) {
            free(monster);
            return NULL;
        }
        monster->type = getMonsterType();
        monster->maxHp = getInt("HP: ");
        monster->hp = monster->maxHp;
        monster->attack = getInt("Attack: ");
    }

    // return created monster or NULL
    return monster;
}

static Item *getItem(GameState *g) {
    // ask user if they want to add an item
    int hasItem = getInt("Add item? (1=Yes, 0=No): ");
    Item *item = NULL;

    // if yes, create new item and get its details from user
    if (hasItem) {
        item = (Item*)safeMalloc(sizeof(Item), g);
        item->name = getString("Item name: ");
        if (item->name == NULL) {
            free(item);
            return NULL;
        }
        item->type = getItemType();
        item->value = getInt("Value: ");
    }

    // return created item or NULL
    return item;
}

static void initPlayer(GameState* g) {
    // check for null game state
    if (g == NULL) {
        return;
    }

    if (g->rooms == NULL) {
        printf("Create rooms first\n");
        return;
    }

    if (g->player != NULL) {
        printf("Player exists\n");
        return;
    }

    // initialize player only if not already done
    Player *player = (Player*)safeMalloc(sizeof(Player), g);
    player->bag = createBST(compareItems, printItem, freeItem);
    if (player->bag == NULL) {
        free(player);
        return;
    }
    player->defeatedMonsters = createBST(compareMonsters, printMonster, freeMonster);
    if (player->defeatedMonsters == NULL) {
        bstFree(player->bag->root, player->bag->freeData);
        free(player->bag);
        free(player);
        return;
    }
    player->maxHp = g->configMaxHp;
    player->hp = player->maxHp;
    player->baseAttack = g->configBaseAttack;
    player->currentRoom = findByID(0, g);
    
    g->player = player;
}

