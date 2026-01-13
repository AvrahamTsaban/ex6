#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "utils.h"

// check all list printers 

static void move(GameState* g);
static void fight(GameState* g);
static Bool battleLoop(Player *player, Monster *monster);
static void pickup(GameState* g);
static void freeRoom(Room* r);
static void checkVictory(GameState* g);
static void viewBag(GameState* g);
static void viewDefeated(GameState* g);

void playGame(GameState* g) {
    if (g == NULL) {
        return;
    }
    if (g->player == NULL) {
        printf("Init player first\n");
        return;
    }

    // game loop
    int choice;
    do {
        Room *room = g->player->currentRoom;
        displayMap(g);
        roomLegend(g);
        printf("\n--- Room %d ---\n", room->id);
        if (room->monster != NULL) {
            printf("Monster: %s (HP:%d)\n", room->monster->name, room->monster->hp);
        }
        if (room->item != NULL) {
            printf("Item: %s\n", room->item->name);
        }
        printf("HP: %d/%d\n", g->player->hp, g->player->maxHp);
        choice = 0;
        choice = getInt("1.Move 2.Fight 3.Pickup 4.Bag 5.Defeated 6.Quit\n");
        if (room->monster != NULL && (choice == 1 || choice == 3)) {
            printf("Kill monster first\n");
            continue;
        }
        switch (choice) {
        case 1:
            move(g);
            break;
        case 2:
            fight(g);
            break;
        case 3:
            pickup(g);
            break;
        case 4:
            viewBag(g);
            break;
        case 5:
            viewDefeated(g);
            break;
        case 6:
            break;
        default:
            break;
        }
    } while (choice != 6);
}

static void move(GameState* g) {
    if (g == NULL || g->player == NULL || g->player->currentRoom == NULL) {
        return;
    }

    // cannot move if monster present
    Room *currentRoom = g->player->currentRoom;

    // get target room based on direction and move there if it exists
    Coordinates coords = {currentRoom->x, currentRoom->y};
    Direction dir = getDir();
    moveCoords(&coords, dir);
    if (coords.x == currentRoom->x && coords.y == currentRoom->y) {
        // moveCoords failed
        return;
    }

    Room *targetRoom = findByCoordinates(coords, g);
    if (targetRoom == NULL) {
        printf("No room there\n");
        return;
    } else {
        g->player->currentRoom = targetRoom;
        if (targetRoom->monster == NULL) {
            targetRoom->visited = 1;
            checkVictory(g);
        }
    }
}

static void fight(GameState* g) {
    if (g == NULL || g->player == NULL || g->player->currentRoom == NULL) {
        return;
    }
    if (g->player->currentRoom->monster == NULL) {
        printf("No monster\n");
        return;
    }

    Player *player = g->player;
    Room *currentRoom = player->currentRoom;
    Monster *monster = currentRoom->monster;

    if (monster->hp <= 0 || monster->name == NULL) {
        // soft failure: remove invalid monster and move on
        freeMonster(monster);
        currentRoom->monster = NULL;
        currentRoom->visited = 1;
        checkVictory(g);
        return;
    }
    if (player->hp <= 0 || player->maxHp <= 0 || (monster->attack <= 0 && player->baseAttack <= 0)) {
        // hard failure: end game
        printf("Invalid stats. Game over.\n");
        freeGame(g);
        exit(0);
    }

    // Let the fighting-lads arise and do a war-dance before us!
    Bool areYouVictorious = battleLoop(player, monster);
    if (areYouVictorious) {
        printf("Monster defeated!\n");
        // add monster to defeated BST, remove from room and check victory
        BST *defeatedBST = player->defeatedMonsters;
        BSTNode *inserted = bstInsert(&(defeatedBST->root), monster, defeatedBST->compare);
        if (inserted == NULL) {
            // should not happen: monster already in defeated BST
            freeMonster(monster);
        }
        currentRoom->monster = NULL;
        currentRoom->visited = 1;
        checkVictory(g);
    } else {
        // player defeated: end game
        printf("--- YOU DIED ---\n");
        freeGame(g);
        exit(0);
    }
}

static Bool battleLoop(Player *player, Monster *monster) {
    // if memory invalid, return FALSE to end the game
    if (player == NULL || monster == NULL) {
        return FALSE;
    }
    
    // Let the young men arise and play before us!
    Bool isYourTurn = TRUE;
    while (player->hp > 0 && monster->hp >0) {
        if (isYourTurn) {
            monster->hp -= player->baseAttack;
            if (monster->hp <= 0) {
                monster->hp = 0;
            }
            printf("You deal %d damage. Monster HP: %d\n", player->baseAttack, monster->hp);
        } else {
            player->hp -= monster->attack;
            if (player->hp < 0) {
                player->hp = 0;
            }
            printf("Monster deals %d damage. Your HP: %d\n", monster->attack, player->hp);
        }
        isYourTurn = (Bool)!isYourTurn;
    }

    // determine victor and return
    return (Bool)(player->hp > 0);
}

static void pickup(GameState* g) {
    if (g == NULL || g->player == NULL || g->player->currentRoom == NULL) {
        return;
    }

    // get item from current room (if exists) and add to player's bag
    Player *player = g->player;
    Item *item = player->currentRoom->item;
    if (item == NULL) {
        printf("No item here\n");
        return;
    }

    BSTNode *inserted = bstInsert(&(player->bag->root), item, player->bag->compare);
    if (inserted != NULL) {
        // successfully inserted: remove item from room
        player->currentRoom->item = NULL;
        printf("Picked up %s\n", item->name);
    } else {
        // two identical items: reject new item
        printf("Duplicate item.\n");
    }
}

static void viewBag(GameState* g) {
    BST *tree = g->player->bag;
    printf("=== INVENTORY ===\n");
    if (tree->root == NULL) {
        printf("\tEmpty\n");
        return;
    } 

    int order = getInt("1.Preorder 2.Inorder 3.Postorder\n");
    switch (order) {
        case 1:
            bstPreorder(tree->root, tree->print);
            break;
        case 2:
            bstInorder(tree->root, tree->print);
            break;
        case 3:
            bstPostorder(tree->root, tree->print);
            break;
        default:
            break;
    }
}

static void viewDefeated(GameState* g) {
    BST *tree = g->player->defeatedMonsters;
    printf("=== DEFEATED MONSTERS ===\n");
    if (tree->root == NULL) {
        printf("\tNone\n");
        return;
    }

    int order = getInt("1.Preorder 2.Inorder 3.Postorder\n");
    switch (order) {
        case 1:
            bstPreorder(tree->root, tree->print);
            break;
        case 2:
            bstInorder(tree->root, tree->print);
            break;
        case 3:
            bstPostorder(tree->root, tree->print);
            break;
        default:
            break;
    }
}

void freeGame(GameState* g) {
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

static void checkVictory(GameState* g) {
    if (g == NULL || g->player == NULL || g->rooms == NULL) {
        return;
    }

    // check if all rooms with monsters have been visited (implicit: all monsters defeated)
    Room *iter = g->rooms;
    while (iter != NULL) {
        if (iter->visited == 0) return;
        iter = iter->next;
    }
    
    printf("***************************************\n             VICTORY!                  \n");
    printf("All rooms explored. All monsters defeated.\n***************************************");
    freeGame(g);
    exit(0);
}

/**********
**Helpers**
**********/

void *safeMalloc(size_t newSize, GameState* g) {
// allocate memory and exit program if allocation fails
    void *ptr = malloc(newSize);
    if (ptr == NULL) {
        freeGame(g);
        exit(1);
    }
    return ptr;
}


Direction getDir() {
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

ItemType getItemType() {
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

MonsterType getMonsterType() {
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
***Rooms***
**search***
**********/

Room *findByID(int id, GameState *g) {
    // check for null game state
    if (g == NULL) {
        return NULL;
    }

    // search for room with given id in game state g
    Room *iter = g->rooms;
    while (iter != NULL && iter->id != id) {
        iter = iter->next;
    }

    return iter;
}

Room *findByCoordinates(Coordinates coords, GameState *g) {
    // check for null game state or empty rooms list
    if (g == NULL || g->rooms == NULL) {
        return NULL;
    }

    // search for room with same coordinates as coords in game state g
    Room *iter = g->rooms;
    while (iter != NULL && (iter->x != coords.x || iter->y != coords.y)) {
        iter = iter->next;
    }
    
    return iter; 
}

void moveCoords(Coordinates *coord, Direction dir) {
    // move coordinates in given direction
    switch (dir) {
        case UP:
            coord->y = coord->y - 1;
            break;
        case DOWN:
            coord->y = coord->y + 1;
            break;
        case LEFT:
            coord->x = coord->x - 1;
            break;
        case RIGHT:
            coord->x = coord->x + 1;
            break;
        }
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
    int** grid = (int**)malloc((size_t)height * sizeof(int*));
    for (int i = 0; i < height; i++) {
        grid[i] = (int*)malloc((size_t)width * sizeof(int));
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

    for (Room *iter = g->rooms; iter != NULL; iter = iter->next) {
        printf("ID %d: [M:%c] [I:%c]\n", iter->id, iter->monster ? EXISTS : N_EXISTS, iter->item ? EXISTS : N_EXISTS);
    }

    printf("===================\n");

}

