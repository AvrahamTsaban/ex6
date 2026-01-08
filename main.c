#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "utils.h"
#include "bst.h"

void addRoom(GameState* g);
void initPlayer(GameState* g);
void freeGame(GameState* g);
typedef void (*ActionFunc)(GameState*);

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
    Room *newRoom = safeMalloc(sizeof(Room));
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
        int attachTo = getInt("Attach to room ID: ");
        if (attachTo > g->roomCount || attachTo <= 0) {
            printf("Invalid ID");
            free(newRoom);
            return;
        }

        // get anchor room by ID and direction to place new room
        Room *anchor = findByID(attachTo, g);
        Direction direction = getInt("Direction (0=Up,1=Down,2=Left,3=Right): ");
        switch (direction) {
        case UP:
            newRoom->x = anchor->x;
            newRoom->y = anchor->y - 1;
            break;
        case DOWN:
            newRoom->x = anchor->x;
            newRoom->y = anchor->y + 1;
            break;
        case LEFT:
            newRoom->x = anchor->x - 1;
            newRoom->y = anchor->y;
            break;
        case RIGHT:
            newRoom->x = anchor->x + 1;
            newRoom->y = anchor->y;
            break;
        default:
            // undefined behaviour, uses wrong error for bug-by-bug compatibility with example code
            printf("Room exists there\n");
            free(newRoom);
            return;
        }
        if (isOccupied(newRoom, g)) {
            printf("Room exists there\n");
            free(newRoom);
            return;
        }
    }

    // set monster and item for new room
    newRoom->monster = getMonster();
    newRoom->item = getItem();

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

static Monster *getMonster() {
    // ask user if they want to add a monster
    int hasMonster = getInt("Add monster? (1=Yes, 0=No): ");
    Monster *monster = NULL;

    // if yes, create new monster and get its details from user
    if (hasMonster) {
        monster = safeMalloc(sizeof(Monster));
        monster->name = getString("Monster name: ");
        monster->type = getInt("Type (0-4): ");
        monster->maxHp = getInt("HP: ");
        monster->hp = monster->maxHp;
        monster->attack = getInt("Attack: ");
    }

    // return created monster or NULL
    return monster;
}

static Item *getItem() {
    // ask user if they want to add an item
    int hasItem = getInt("Add item? (1=Yes, 0=No): ");
    Item *item = NULL;

    // if yes, create new item and get its details from user
    if (hasItem) {
        item = safeMalloc(sizeof(Item));
        item->name = getString("Item name: ");
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
        Player *player = safeMalloc(sizeof(Player));
        player->bag = createBST(compareItems, printItem, freeItem);
        player->defeatedMonsters = createBST(compareMonsters, printMonster, freeMonster);
        player->maxHp = g->configMaxHp;
        player->hp = player->maxHp;
        player->baseAttack = g->configBaseAttack;
        player->currentRoom = 0;
        
        g->player = player;
    }
}

static void freeGame(GameState* g) {
    // free player and its BSTs, then rooms list
    if (g == NULL) {
        return;
    }

    if (g->player != NULL) {
        if (g->player->bag != NULL) {
            bstFree(g->player->bag->root, g->player->bag->freeData);
            free(g->player->bag);
        }
        if (g->player->defeatedMonsters != NULL) {
            bstFree(g->player->defeatedMonsters->root, g->player->defeatedMonsters->freeData);
            free(g->player->defeatedMonsters);
        }
        free(g->player);
    }

    Room *tmp, *iter = g->rooms;
    while (iter != NULL) {
        tmp = iter;
        iter = iter->next;
        freeRoom(tmp);
    }
}

static void freeRoom(Room* r) {
    // free monster and item in room, then room itself
    if (r == NULL) {
        return;
    }

    if (r->monster != NULL) {
        freeMonster(r->monster);
    }

    if (r->item != NULL) {
        freeItem(r->item);
    }

    free(r);
}

