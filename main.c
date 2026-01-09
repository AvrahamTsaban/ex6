#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "utils.h"
#include "bst.h"

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
    Room *newRoom = safeMalloc(sizeof(Room), g);
    newRoom->visited = 0;
    newRoom->next = NULL;

    // set room coordinates
    if (g->roomCount == 0) {
        newRoom->x = 0;
        newRoom->y = 0;
    } else {
        // display map and legend for user to choose attachment point
        displayMap(g);
        roomLegend(g);

        // get anchor room ID and direction to place new room
        int attachTo = getInt("Attach to room ID: ");
        Room *anchor = findByID(attachTo, g);
        if (attachTo > g->roomCount || attachTo <= 0 || anchor == NULL) {
            printf("Invalid ID");
            free(newRoom);
            return;
        }

        Direction direction = getInt("Direction (0=Up,1=Down,2=Left,3=Right): ");
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
    }

    // set monster and item for new room
    newRoom->monster = getMonster(g);
    newRoom->item = getItem(g);

    // assign ID, increase room count by 1 and add new room to game state's rooms list
    newRoom->id = ++(g->roomCount);
    if (g->rooms == NULL) {
        g->rooms = newRoom;
    } else {
        Room *iter = g->rooms;
        while (iter->next != NULL) {
            iter = iter->next;
        }
        iter->next = newRoom;
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
        monster = safeMalloc(sizeof(Monster), g);
        monster->name = getString("Monster name: ", g);
        monster->type = getInt("Type (0-4): ");
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
        item = safeMalloc(sizeof(Item), g);
        item->name = getString("Item name: ", g);
        item->type = getInt("Type (0=Armor, 1=Sword): ");
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

    // initialize player only if not already done
    if (g->player == NULL) {
        Player *player = safeMalloc(sizeof(Player), g);
        player->bag = createBST(compareItems, printItem, freeItem);
        player->defeatedMonsters = createBST(compareMonsters, printMonster, freeMonster);
        player->maxHp = g->configMaxHp;
        player->hp = player->maxHp;
        player->baseAttack = g->configBaseAttack;
        player->currentRoom = 0;
        
        g->player = player;
    }
}

