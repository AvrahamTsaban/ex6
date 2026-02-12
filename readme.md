# Exercise 6 — Text-Based RPG Dungeon Game

## Overview

A text-based dungeon-crawling RPG implemented in C as a multi-file project. Players navigate connected rooms, fight monsters, collect items, and track progress using Binary Search Trees. The project covers BSTs, linked lists, function pointers, generic pointers, enums, and modular design.

## Author

Avraham Tsaban

## Compilation

### Using Makefile (GCC — default)

```bash
make
```

Uses GCC with strict warnings (`-Wall -Wextra -Werror -std=c99 -g`), additional pedantic flags, and `-Og` optimization for debugging.

```bash
make clean    # remove build artifacts
```

### Using Makefile.clang (Clang — alternative)

```bash
make -f Makefile.clang
```

Uses Clang with `-Weverything -Werror`, hardening flags (`-fstack-protector-strong`, `-D_FORTIFY_SOURCE=3`), and `-O3` optimization. See the Makefile for a note on switching to `-std=c99` for course compliance.

```bash
make -f Makefile.clang clean
```

### Direct compilation (per exercise instructions)

```bash
gcc -Wall -Wextra -Werror -std=c99 -g main.c game.c bst.c utils.c -o program
```

## Running

```bash
./program <player_hp> <base_attack>
```

Example:

```bash
./program 100 15
```

## Gameplay

### Main Menu

```
=== MENU ===
1. Add Room
2. Init Player
3. Play
4. Exit
```

### In-Game Actions

```
1. Move      — Navigate to adjacent rooms
2. Fight     — Battle the monster in current room
3. Pickup    — Collect items
4. Bag       — View collected items (BST in-order)
5. Defeated  — View defeated monsters (BST in-order)
6. Quit      — Return to main menu
```

### Monster Types

Phantom, Spider, Demon, Golem, Cobra — each with unique stats.

### Item Types

Armor (increases defense/HP) and Sword (increases attack damage).

### Victory

Visit all rooms and defeat all monsters to win.

## Code Structure

The project is split across 4 source files and 3 headers.

### Data Types (`game.h`)

**Enums:** `ItemType` (ARMOR, SWORD), `MonsterType` (PHANTOM, SPIDER, DEMON, GOLEM, COBRA), `Direction` (UP, DOWN, LEFT, RIGHT), `Bool` (FALSE, TRUE).

**Structs:**

| Struct | Key Fields |
|--------|------------|
| `Item` | `name`, `type`, `value` |
| `Monster` | `name`, `type`, `hp`, `maxHp`, `attack` |
| `Room` | `id`, `x`/`y`, `visited`, `monster*`, `item*`, `next` (linked list) |
| `Player` | `hp`, `maxHp`, `baseAttack`, `bag` (BST), `defeatedMonsters` (BST), `currentRoom` |
| `GameState` | `rooms`, `player`, `roomCount`, `configMaxHp`, `configBaseAttack` |

### Generic BST (`bst.h` / `bst.c`)

Uses `void*` data with function pointers for comparison, printing, and freeing:

| Function | Description |
|----------|-------------|
| `createBST(cmp, print, freeData)` | Allocate a BST with callback functions. |
| `bstInsert(root, data, cmp)` | Insert data; returns `NULL` on duplicate. |
| `bstFind(root, data, cmp)` | Search by comparison function. |
| `bstInorder(root, print)` | In-order traversal (LNR). |
| `bstPreorder(root, print)` | Pre-order traversal (NLR). |
| `bstPostorder(root, print)` | Post-order traversal (LRN). |
| `bstFree(root, freeData)` | Recursively free all nodes and their data. |

### Game Logic (`game.c`)

| Function | Description |
|----------|-------------|
| `playGame(g)` | Main game loop — map display, action menu (move/fight/pickup/bag/defeated/quit). |
| `move(g)` | *(static)* Move player to adjacent room; blocks if monster is present. |
| `fight(g)` | *(static)* Initiate combat with the room's monster. |
| `battleLoop(player, monster)` | *(static)* Turn-based combat loop; returns TRUE if player wins. |
| `pickup(g)` | *(static)* Pick up room's item into player's bag BST. |
| `viewBag(g)` / `viewDefeated(g)` | *(static)* Display BST contents in user-chosen traversal order. |
| `checkVictory(g)` | *(static)* Check if all rooms visited and all monsters defeated. |
| `freeGame(g)` | Free entire game state (player, BSTs, all rooms). |
| `safeMalloc(size, g)` | `malloc` wrapper — frees game and exits on failure. |
| `findByID(id, g)` / `findByCoordinates(coords, g)` | Room lookup in linked list. |
| `displayMap(g)` / `roomLegend(g)` | Print spatial grid of rooms / per-room monster/item presence. |
| `moveCoords(coord, dir)` | Apply directional offset to coordinates. |

### Main Entry Point (`main.c`)

| Function | Description |
|----------|-------------|
| `main(argc, argv)` | Parses `<player_hp>` and `<base_attack>` args, runs menu loop. |
| `addRoom(g)` | *(static)* Creates a room: prompts for anchor, direction, monster, item. |
| `getMonster(g)` / `getItem(g)` | *(static)* Optionally creates a monster or item from user input. |
| `initPlayer(g)` | *(static)* Allocates player with empty BSTs; places at room 0. |

### Utilities (`utils.c` / `utils.h`)

| Function | Description |
|----------|-------------|
| `getInt(prompt)` | Validated integer input. |
| `getString(prompt)` | Dynamically reads a string until newline (auto-expanding buffer). |
| `semiSafeRealloc(ptr, size)` | `realloc` wrapper; frees old pointer on failure. |
| `getDir()` / `getItemType()` / `getMonsterType()` | Validated enum input prompts. |
| `compareMonsters(a, b)` / `compareItems(a, b)` | BST comparison callbacks (by name, then other fields). |
| `printMonster(data)` / `printItem(data)` | BST print callbacks. |
| `freeMonster(data)` / `freeItem(data)` | BST free callbacks. |

## Project Files

| File | Description |
|------|-------------|
| `main.c` | Entry point, menu system, room and player initialization |
| `game.c` / `game.h` | Core game logic, combat, movement, victory conditions |
| `bst.c` / `bst.h` | Generic Binary Search Tree implementation (using `void*`) |
| `utils.c` / `utils.h` | Utility functions for I/O and memory management |
| `Makefile` | GCC build configuration |
| `Makefile.clang` | Clang build configuration with hardening flags |
| `ex6.example` | Reference Linux executable provided by the TA |
| `ex6_instructions.md` | Exercise instructions |
| `input.txt` / `output.txt` | Sample input and expected output |
| `.vscode/` | VS Code configuration for debugging (see below) |

## VS Code Debugging

The `.vscode/` directory contains configurations tailored for debugging in VS Code:

- **`tasks.json`** — Build task that compiles the project with GCC and debug symbols.
- **`launch.json`** — Debug configuration using GDB (`cppdbg`), launching the program with sample arguments and a pre-launch build step.

These files allow setting breakpoints and stepping through the code directly in VS Code.

## Code Ownership

The base implementation and design originate from the TA's [initial commit](https://github.com/CSI-BIU/ex6/commit/86052561dec8a889767fe80c1d644ff45553ba52). The student built upon this scaffolding. Both the TA's and the student's code are present in the repository.

## Attribution

The exercise design, specifications, instructions, scaffolding code, and sample I/O were created by **Eliyahu Houri**, the Teaching Assistant responsible for this assignment. The instructions file (`ex6_instructions.md`) and the reference executable (`ex6.example`) are his work. Any license in this repository applies only to the student's code implementation and not to the exercise design or TA-provided materials.
