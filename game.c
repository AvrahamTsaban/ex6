#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "bst.h"
#include "utils.h"



void playGame(GameState* g) {
    
    do {
        int choice = getint("")
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


