#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "bst.h"
#include "utils.h"



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
        printf("--- Room 0 ---\nHP: 100/100", g->player->hp, g->player->maxHp);
        choice = 0;
        choice = getInt("1.Move 2.Fight 3.Pickup 4.Bag 5.Defeated 6.Quit\n");
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

static void fight(GameState* g) {
    if (g == NULL || g->player == NULL || g->player->currentRoom == NULL) {
        return;
    }
    if (g->player->currentRoom->monster == NULL) {
        printf("No monster\n");
        return;
    }

    Player *player = g->player;
    Monster *monster = g->player->currentRoom->monster;

    if (monster->hp <= 0 || monster->name == NULL || monster->attack < 0) {
        // todo: soft failure: some func to remove monster
        return;
    }
    if (player->hp <= 0 == NULL || player->baseAttack <= 0 || player->maxHp <= 0) {
        // todo: soft failure: some func to end game
    }

    // battle loop: 
    // todo: איך כותבים באנגלית "ישחקו הנערים לפנינו"
    int result = battleLoop(player, monster);
}

static int battleLoop(Player *player, Monster *monster) {
    if (player == NULL || monster == NULL) {
        return;
    }

    Bool isYourTurn = 1;
    while (player->hp > 0 && monster->hp >0) {

    }
    
}

static void pickup(GameState* g) {
    // placeholder
    printf("Picking up...\n");
    /*
    TODO:
    - NULL pointer checks
    - if enemy present, cannot pick up: "Kill monster first\n" and return
    - check if item present in current room: if not, ?
    - pick up item: add to player's bag BST and remove from room
    */
}

static void move(GameState* g) {
    if (g == NULL || g->player == NULL || g->player->currentRoom == NULL) {
        return;
    }

    // cannot move if monster present
    Room *currentRoom = g->player->currentRoom;
    if (currentRoom->monster != NULL) {
        printf("Kill monster first\n");
        return;
    }

    // get target room based on direction and move there if it exists
    Coordinates coords = {currentRoom->x, currentRoom->y};
    Direction dir = getDir();
    moveCoords(&coords, dir);
    Room *targetRoom = findByCoordinates(coords, g);
    if (targetRoom == NULL || (coords.x == currentRoom->x && coords.y == currentRoom->y)) {
        printf("No room there\n");
        return;
    } else {
        g->player->currentRoom = targetRoom;
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


