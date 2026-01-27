# Exercise 6 - Text-Based RPG Dungeon Game

A text-based role-playing game implemented in C, featuring room exploration, monster combat, item collection, and inventory management using Binary Search Trees (BST).

## Table of Contents

- [Description](#description)
- [Features](#features)
- [Project Structure](#project-structure)
- [Compilation Instructions](#compilation-instructions)
  - [Using Make with GCC (Default)](#using-make-with-gcc-default)
  - [Using Make with Clang](#using-make-with-clang)
  - [Direct Compilation with GCC](#direct-compilation-with-gcc)
  - [Direct Compilation with Clang](#direct-compilation-with-clang)
- [Usage](#usage)
- [Game Mechanics](#game-mechanics)
- [Reference Implementation](#reference-implementation)
- [License Disclosure](#license-disclosure)

---

## Description

This program implements a dungeon-crawling RPG game where players navigate through connected rooms, fight monsters, collect items, and track their progress. The game uses a linked list for room management and binary search trees for inventory (bag) and defeated monsters tracking.

## Features

- **Room System**: Create and connect rooms in a 2D grid layout
- **Combat System**: Turn-based combat with various monster types (Phantom, Spider, Demon, Golem, Cobra)
- **Item System**: Collect armor and swords to enhance player stats
- **Inventory Management**: BST-based bag system for collected items
- **Monster Tracking**: BST-based system for tracking defeated monsters
- **Map Display**: Visual representation of explored dungeon layout
- **Victory Condition**: Visit all rooms and defeat all monsters

## Project Structure

```
ex6/
├── main.c          # Main entry point, menu system, room/player initialization
├── game.c          # Core game logic, combat, movement, victory conditions
├── game.h          # Game structures (Player, Room, Monster, Item, GameState)
├── bst.c           # Binary Search Tree implementation
├── bst.h           # BST interface definitions
├── utils.c         # Utility functions for I/O and memory management
├── utils.h         # Utility function declarations
├── Makefile        # Default GCC build configuration
├── Makefile.clang  # Alternative Clang build configuration
├── ex6_linux       # Reference implementation binary (provided by TA)
├── input.txt       # Sample input file
├── output.txt      # Sample output file
└── readme.md       # This documentation
```

---

## Compilation Instructions

### Using Make with GCC (Default)

The default Makefile uses GCC with strict warning flags:

```bash
make
```

Or explicitly:

```bash
make -f Makefile
```

**Compiler flags used:**
- `-Wall -Wextra -Werror` - Enable all warnings and treat as errors
- `-std=c99` - Use C99 standard
- `-g` - Include debug symbols
- `-Wpedantic -Wshadow -Wformat=2` - Additional strict warnings
- `-Wstrict-overflow=2 -Wwrite-strings -Wcast-align -Wpointer-arith`
- `-Wconversion` - Warn on implicit type conversions
- `-Og` - Optimization level for debugging

To clean build artifacts:

```bash
make clean
```

### Using Make with Clang

An alternative Makefile is provided for Clang compilation with extensive warning coverage:

```bash
make -f Makefile.clang
```

**Compiler flags used:**
- `-std=c99` - Use C99 standard
- `-Weverything -Werror` - Enable all Clang warnings and treat as errors
- `-Og -g` - Debug optimization and symbols
- `-fstack-protector-strong` - Stack buffer overflow protection
- `-D_FORTIFY_SOURCE=3` - Additional runtime security checks

To clean:

```bash
make -f Makefile.clang clean
```

### Direct Compilation with GCC

If you prefer not to use Make, compile directly with GCC:

**Basic compilation:**

```bash
gcc -Wall -Wextra -Werror -std=c99 -g main.c game.c bst.c utils.c -o program
```

**With additional warnings (recommended):**

```bash
gcc -Wall -Wextra -Werror -std=c99 -g -Wpedantic -Wshadow -Wformat=2 -Wconversion -Og main.c game.c bst.c utils.c -o program
```

### Direct Compilation with Clang

**Basic compilation:**

```bash
clang -Wall -Wextra -Werror -std=c99 -g main.c game.c bst.c utils.c -o program
```

**With strict warnings:**

```bash
clang -std=c99 -Weverything -Werror -Wno-declaration-after-statement -Wno-padded -g main.c game.c bst.c utils.c -o program
```

---

## Usage

Run the compiled program with two required arguments:

```bash
./program <player_hp> <base_attack>
```

**Parameters:**
- `player_hp` - Maximum hit points for the player
- `base_attack` - Base attack damage for the player

**Example:**

```bash
./program 100 15
```

### Main Menu

Upon starting, you'll see the main menu:

```
=== MENU ===
1.Add Room
2.Init Player
3.Play
4.Exit
```

### Gameplay Flow

1. **Add Rooms** - Create dungeon rooms, optionally adding monsters and items
2. **Init Player** - Initialize the player character (rooms must exist first)
3. **Play** - Enter the game and explore the dungeon

### In-Game Options

During gameplay:

```
1.Move     - Navigate to adjacent rooms
2.Fight    - Battle the monster in current room
3.Pickup   - Collect items from current room
4.Bag      - View collected items
5.Defeated - View defeated monsters
6.Quit     - Return to main menu
```

---

## Game Mechanics

### Monster Types

| Type    | Description |
|---------|-------------|
| PHANTOM | Ethereal enemy |
| SPIDER  | Creepy crawler |
| DEMON   | Fiery foe |
| GOLEM   | Stone guardian |
| COBRA   | Venomous serpent |

### Item Types

| Type  | Effect |
|-------|--------|
| ARMOR | Increases defense/HP |
| SWORD | Increases attack damage |

### Combat

- Turn-based battle system
- Player attacks first, then monster retaliates
- Combat continues until one side's HP reaches 0
- Defeated monsters are stored in a BST (in-order by name)
- Player death results in game over

### Victory

Win by visiting all rooms and defeating all monsters!

---

## Reference Implementation

The `ex6_linux` file is a **reference implementation binary provided by the Teaching Assistant (TA)**. It demonstrates expected program behavior and can be used to verify your implementation's correctness.

To run the reference implementation:

```bash
chmod +x ex6_linux
./ex6_linux <player_hp> <base_attack>
```

---

## License Disclosure

> **⚠️ IMPORTANT NOTICE**
>
> The base implementation and design of this project originates from commit [`86052561dec8a889767fe80c1d644ff45553ba52`](https://github.com/CSI-BIU/ex6/commit/86052561dec8a889767fe80c1d644ff45553ba52) and is **NOT owned by me**.
>
> Any license included in this repository **may not apply** to the code and design that existed at or before that commit. The original base implementation may be subject to different terms, including but not limited to academic institution policies, course material restrictions, or other proprietary rights.
>
> This disclosure is made in good faith to clarify ownership and licensing boundaries. If you are the original author or rights holder and have concerns, please open an issue or contact the repository maintainer.

---

## Author

- **Name:** Avraham Tsaban
- **Assignment:** Exercise 6
