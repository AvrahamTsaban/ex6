**EX6  2026**

General instructions  
The T.A in charge for this exercise is **Eliyahu Houri**.

* You must fork the repository ex6 from the course’s git.  
* Change the files according to instructions.  
* After completing all the requirements, submit your work to the [submit system](https://submit.cs.biu.ac.il/cgi-bin/welcome.cgi) under **ex6** until **22.1.26 at 23:59:59**.  
* Your submission should be a zip file containing:  
  * main.c  
  * game.c  
  * bst.c  
  * utils.c  
  * game.h  
  * bst.h  
  * utils.h  
* Late submissions will be fined accordingly:  
  * One day \- 10 points. (the maximum grade is 90\)  
  * Two days \- 20 points.  
  * Three days \- 30 points.  
  * **You can’t submit this or any other exercise more than 3 days late.**  
* Please post any questions about the exercise **publicly** in the course forum.  
* Requests for an extension (for justified reasons only) should be sent to the email: [CSI.BIU2024@gmail.com](mailto:CSI.BIU2024@gmail.com) **with:**  
* Full Name.  
* Your Submit system username.  
* ID.

The input and output design instructions must be read carefully, exactly according to the attached examples.  
There are executable programs (windows and Linux) on the course site, you can run them on your computer and it may help you understand better.  
The automation grading system checks that your output is the same as the expected output and therefore your output must be **exactly the same** as the examples.  
As so, there is a Coding Style file in the forum \- read it and work accordingly.  
Testing your code on the server is highly recommended to ensure there are no warnings or errors.  
An exercise that contains warning(s) or error(s) will receive a fine that can reach 100 points.  
You are reminded that thinking together is a blessing, working together is cheating.  
An exercise that the system identifies as a “copy” will result in **0** \- for everyone involved, and additional actions will take place.

In the development of your code, you can use any work environment.  
The main thing is you know how to take the code files from this environment, check them on the university servers, and submit them using the submission system.  
Please note that you submit only the files containing your code, and not unnecessary files created by the work environment.  
Also, **DO NOT** submit files/folders with names containing Hebrew characters.  
Please note that it is not possible to copy a file with a path containing Hebrew characters to the university's servers.

This exercise **compile** command (Linux) is:  
gcc \-Wall \-Wextra \-Werror \-std=c99 \-g main.c game.c bst.c utils.c

This exercise **run** command is:  
./a.out \<player\_hp\> \<player attack\>  
---

Mission instructions

This exercise will address several topics, including:  
1\. BST.  
2\. Linked lists.  
3\. Pointers, including pointers to functions and generic pointers.  
4\. Generic functions.  
5\. Arguments to main.  
6\. Enums.  
7\. Modularity.

The exercise is to create and play a small [dungeon crawler game](https://en.wikipedia.org/wiki/Dungeon_crawl).  
How would you do it?  
We will use few files for that:  
1\. **main.c** \- in which we will have the main function.  
2\. **game.c** and **game.h** \- in which we will have the logic of the game.  
3\. **bst.c** and **bst.h** \- in which we will have the logic of the BST.  
4\. **utils.c** and **utils.h** \- in which we will have utils functions.   
**All the h files and main.c, utils.c** are given to you in here: [https://github.com/CSI-BIU/ex6](https://github.com/CSI-BIU/ex6)  
Just fork the repository and work on the files on your computer.  
Also, you are getting the display map function in the game.c.

**ADD COMMENTS IN THESE FILES**, it will help you understand what is going on in each function.

The instructions are written to help you build the game step by step, so read them all carefully and work through them in order.  
Anyway, you obviously can take the wheel and do it in a way you think will be the best.

**Note that you do not have to use the functions given to you, but the tree has to be generic\!**

---

General Q\&A  
**Q:** What libraries can I use?  
**A:** Only: stdio.h, string.h, stdlib.h  
**Q:** Can I use all the functions in string.h?   
**A:** Only the ones we saw in the triguls, and [strdup](https://en.cppreference.com/w/c/experimental/dynamic/strdup).  
**Q:** Is there any limit on the inputs in the ex?  
**A:** No.  
**Q:** Do I just need to have the same output?  
**A:** No. You also need to free ALL BITES. even if a single one is leaked, the test result will be 0\.  
**Q:** Can I assume anything?  
**A:** There is “you can assume” in the relevant parts.  
**Q:** Can I add or change functions?  
**A:** Sure, in any file.  
---

The game \- tl;dr  
We will have two steps in this game \- create the game, and play it.  
The game has a map and a player.  
The map consists of rooms, and each room may have a monster and an item.  
The player will fight this monster and may pick this item after winning the monster.  
The game will end with the player losing all its health, or visiting and clearing all rooms from monsters.  
---

**game.h**  
Enums  
You are given 2 enums, which you cannot change. you can add more enums if you want.  
typedef enum { ARMOR, SWORD } ItemType;  
typedef enum { PHANTOM, SPIDER, DEMON, GOLEM, COBRA } MonsterType;  
---

Structs  
You are given a few structs, which you cannot change. 🙂  
We will go one by one and explain each field. You may want to read it once, then read the rest of the document, and come back to reading it again.  
Item  
typedef struct Item {  
   char\* name;  
   ItemType type;  
   int value;  
} Item;

**name**: (char\*, dynamically allocated) The name is the name of the item.  
**type**: (ItemType) This is the type of the item, whether a sword, or an armor.  
**value**: (int) The value of the item..  
Monster  
typedef struct Monster {  
   char\* name;  
   MonsterType type;  
   int hp;  
   int maxHp;  
   int attack;  
} Monster;

**name**: (char\*, dynamically allocated) The name is the name of the monster.  
**type**: (MonsterType) This is the type of the monster, which is one of these: *PHANTOM, SPIDER, DEMON, GOLEM, COBRA.*  
**hp**: (int) This is the current health points (hp) of the monster.  
**maxHp**: (int) This is the maximum health points of the monster. Which you can think of as the initial hp of the monster.  
**attack**: (int) This is how strong a hit from the monster is.  
Room  
typedef struct Room {  
   int id;  
   int x, y;  
   int visited;  
   Monster\* monster;        
   Item\* item;  
   struct Room\* next;  
} Room;

**id**: (int) This is a unique ID for the room, which is automatically generated when one creates a room, in ascending order.  
**x,y**: (int) Those are coordinates of the room. Why? you will see the use of them.  
**visited**: (int) A flag to track whether the room has been visited, used to determine if the player has won the game (If he also killed all the monsters).  
**monster**: (Monster\*) A monster in that room.  
**item**: (Item\*) An item in that room.  
**next**: (Room\*) the next room in the  linked list of rooms. It will be easier to handle the rooms with it.  
---

Player  
typedef struct Player {  
   int hp;  
   int maxHp;  
   int baseAttack;  
   BST\* bag;  
   BST\* defeatedMonsters;  
   Room\* currentRoom;  
} Player;

**hp**: (int) This is the current health points (hp) of the player.  
**maxHp**: (int) This is the initial health points (hp) of the player.  
**baseAttack**: (int) This is the base attack of the player.  
**bag**: (BST\*) A pointer to the root of the bag (The bag is a Binary Search Tree).  
**defeatedMonsters**: (BST\*) A pointer to the root of a tree that holds all the defeated monsters the player slain.  
**currentRoom**: (Room\*) A pointer to the current room the player is in.  
---

GameState  
This struct will help us track the game state in each given time.  
typedef struct {  
   Room\* rooms;  
   Player\* player;  
   int roomCount;  
   int configMaxHp;  
   int configBaseAttack;  
} GameState;

**rooms**: (Room\*) The head of the room linked list.  
**player**: (Player\*) A pointer to the player.  
**roomCount**: (int) The number of rooms we currently have.  
**configMaxHp**: (int) The max hp of the player that we get from the main arguments.  
**configBaseAttack**: (int) The attack value of the player that we get from the main arguments.  
---

**bst.h**  
Enums  
This struct is a node in a Binary Search Tree.  
typedef struct BSTNode {  
   void\* data;  
   struct BSTNode\* left;  
   struct BSTNode\* right;  
} BSTNode;

**data**: (void\*) We have two BST’s, one for items and one for monsters, so for using the same struct for both data types, we need to use void\*, as we saw in the Tirgul.  
**left**: (BSTNode\*) A pointer to the left child.  
**right**: (BSTNode\*) A pointer to the right child.  
---

This struct is a wrapper for a BST.  
typedef struct {  
   BSTNode\* root;  
   int (\*compare)(void\*, void\*);  
   void (\*print)(void\*);  
   void (\*freeData)(void\*);  
} BST;

**root**: (BSTNode\*) A pointer to the tree;s root node.  
**compare**: int(\*compare)(void\*, void\*) Function pointer to compare two elements in the tree.  
**print**: void(\*print)(void\*) Function pointer to print an element.  
**freeData**: void(\*freeData)(void\*) Function pointer to free an element.  
---

Starting the program  
 The program expects 2 arguments:  
`./ex6.out <player_hp> <base_attack>`

**player\_hp** is the hp of the player that will be determined by the user, according to the first argument to the main.  
**base\_attack** is the attack of the player that will be determined by the user, according to the second argument to the main.  
The logic to handle these arguments is given in the main you got.  
But, for your own good, understand what is happening there.  
---

Creating the game  
The main menu is the menu from where we will access everything in the program.  
After printing the “Loaded Assests…” and going down a line, we will print the following menu:  
`=== MENU ===`  
`1.Add Room`  
`2.Init Player`  
`3.Play`  
`4.Exit`

*Creating the game \- Add Room*  
When adding a room, we will first print the spatial map and the room legend.  
For example:  
`=== SPATIAL MAP ===`  
        `[ 1]`  
`[ 3][ 2][ 0]`  
`=== ROOM LEGEND ===`  
`ID 3: [M:V] [I:V]`  
`ID 2: [M:X] [I:X]`  
`ID 1: [M:X] [I:V]`  
`ID 0: [M:V] [I:X]`  
`===================`  
The output shown represents four rooms connected in sequence. Some of the rooms contain monsters and items, while others do not.  
Again, the function responsible for printing the spatial map is given to you.  
About the Room Legend \- you need to implement it.  
How do I know if a room has a monster or an item?  
**M** is for monster.  
**I** is for item.  
If a room has a monster in it, we will print V, otherwise \- X.  
If a room has an item in it, we will print V, otherwise \- X.

So, what if it is the first room? We will not print anything, and continue to the next step.  
Then, we will ask the user where he wants to attach the new room.  
`Attach to room ID:`  
Here, you can assume the user will choose a room we have.  
Then, we will ask in what direction the user wants to attach the new room to the old room.  
`Direction (0=Up,1=Down,2=Left,3=Right):`  
Here, you can assume the user will choose 0-3, but \- this direction may be taken.  
If it is not taken, we will add a new room to the asked direction of the old room.  
If it is taken, we will prompt:  
`Room exists there`  
And go back to the main menu.  
So, what if it is the first room? we will just add room id 0, in coordinates (0,0).

**Please note \- each room has coordinates, When the first room is in (0,0).**  
The coordinates are a little different from what you know.  
Left and right are still the same: right is \+1 to the x axis, and left is \-1 to the x axis.  
But up is \-1 to the y axis, and Down is \+1 to the y axis.

Then, we will give the user an option to add a monster and an item:  
`Add monster? (1=Yes, 0=No):`  
If the user chose 0, we will continue to the next step \- adding an item.  
If the user chose 1, We will prompt, one after another, the following:  
`Monster name:`   
`Type (0-4):`   
`HP:`   
`Attack:` 

**Monster name-** Can be any string, unlimited in size.  
**Type** \- you can assume the user will enter 0 to 4\.  
**HP** \- you can assume the user will enter a positive Integer.  
**Attack** \- you can assume the user will enter a positive Integer.

Immediately after that, we will prompt:  
`Add item? (1=Yes, 0=No):`  
And again, on 0, we will just continue to the next step.  
On 1, we will print the following, one after another:  
`Item name:`  
`Type (0=Armor, 1=Sword):`   
`Value:` 

**Item name** \- Can be any string, unlimited in size.  
**Type** \- you can assume the user will enter 0 or 1\.  
**Value** \- you can assume the user will enter a positive Integer.

After that, we will print:  
`Created room <room id> at (<x coordinate>,<y coordinate>)`  
For example:  
`Created room 4 at (1,0)`  
So, what if it is the first room?   
We will prompt the appropriate values:   
`Created room 0 at (0,0)`

*Creating the game \- Init Player*  
Back to the main menu:  
`=== MENU ===`  
`1.Add Room`  
`2.Init Player`  
`3.Play`  
`4.Exit`

When choosing a 2- init player, we will create a player.  
We will ask for nothing from the user.  
We can do this option only after there is at least 1 room.  
If there is no rooms, we will output:  
`Create rooms first`  
---

Playing the game  
Back to the main menu:  
`=== MENU ===`  
`1.Add Room`  
`2.Init Player`  
`3.Play`  
`4.Exit`

We can choose to play, only after we have rooms and a player.  
If we don’t have a player, we will output:  
`Init player first`  
\[There is no situation in which we have a player and no rooms\].

When choosing a 3- play, we will prompt the following menu:

We will first print the spatial map and the room legend, and then, some details about the room we are in, and how much hp the player has.  
For example:  
`=== SPATIAL MAP ===`  
`[ 0][ 1]`  
`=== ROOM LEGEND ===`  
`ID 1: [M:X] [I:X]`  
`ID 0: [M:V] [I:V]`  
`===================`  
`--- Room 0 ---`  
`Monster: Jhony (HP:13)`  
`Item: Midas`  
`HP: 100/100`

In this situation, we have two rooms, 0 and 1\.  
The player is currently in room 0, so we also printing details about the room:  
`Monster: Jhony (HP:13)` \- Jhony is the name of the monster we chose, and HP is the hp of the monster.  
`Item: Midas` \- Midas is the name of the item.  
`HP: 100/100` \- This is the player's current hp, and the player’s max hp.  
How did we get the max hp? from the first main argument.  
Then, we will print the sub menu:  
`1.Move 2.Fight 3.Pickup 4.Bag 5.Defeated 6.Quit`  
You can assume the user is choosing only 1-6.

About the room legend \- you need to print it in descending order, as in the example above.

*Playing the game \- move*  
The player is able to move between any adjacent rooms, even if we didn’t connect them specifically.  
So, as long as the rooms are adjacent, we can move between them.  
**And, the player can move from a room only if there is no monster there. Otherwise, it has to kill the monster first.**  
If we tried to move when there is a monster, we will print:  
`Kill monster first`  
And the spatial map, room legend, details, and menu again.  
If we are able to move, we will print:  
`Direction (0=Up,1=Down,2=Left,3=Right):`  
If the player chose a direction that we have a room in that direction, all good \- and we will move to that room.  
otherwise, we will print:  
`No room there`  
And the spatial map, room legend, details, and menu again.

*Playing the game \- Fight*  
If we chose to fight and there is no monster, we will just print:  
No monster  
If there is a monster, we will start a turn based routine, when the player attacks first.  
For example:  
`You deal 5 damage. Monster HP: 8`  
`Monster deals 3 damage. Your HP: 97`  
`You deal 5 damage. Monster HP: 3`  
`Monster deals 3 damage. Your HP: 94`  
`You deal 5 damage. Monster HP: 0`  
`Monster defeated!`

In each player turn, we will print how much damage we did, and what is the hp of the monster after it.  
Please note \- the damage we are doing is the player’s attack value.  
In each monster turn, we will print how much damage it did, and what is the hp of the player after it.  
Please note \- the damage the monster doing is the monster’s attack value.

If we won (we killed the monster before it killed us), we will print:  
`Monster defeated!`  
As you can in the output above.  
And, we will add it to the BST of the defeated monsters, as we will talk below.

If the monster won, we will end the program **AFTER FREEING ALL MEMORY**  
And print:  
`--- YOU DIED ---`

*Playing the game \- Pickup*  
When we choose Pickup, but there is still a monster in the room, we will print:  
`Kill monster first`  
If there is no monster, but also there is no item in the room, we will print:  
`No item here`  
If there is one, we will pick it up, add it to the BST of items, and print:  
`picked up <item name>`  
For example:  
`Picked up Midas`  
If we already have the **exact same** item in the bag, we will print:  
`Duplicate item.`  
And do nothing. (the item will stay in that room).

*Playing the game \- Bag*  
When we choose Bag, we will print:  
`=== INVENTORY ===`  
`1.Preorder 2.Inorder 3.Postorder`  
And then, we will print the Bag, which is a BST of items, according to the order the user chose.

But, how is the tree ordered in the first place?

1\. name (strcmp)  
2\. If names equal: value  
3\. If values equal: type (ARMOR=0 before SWORD=1)

Which means, a bigger **name** (according to strcmp) will be on the right, and smaller, will be on the left.  
If two names are equal, we will not stop here, but we will continue to compare the **value** of the item \- bigger to the right, and smaller to the left.  
If two values are equal, we will continue to compare based on the **enum** \- armor before sword.  
If they are Identical, we are handling it in the pickup option (not picking up).  
Example of Bag output in preorder.

	`[ARMOR] A - Value: 3`  
`[SWORD] A - Value: 2`  
`[SWORD] A - Value: 3`

And you can deduce from the rules that:  
The root is the first one, then, the second print is on the left (same name, smaller value).  
And the third print is on the right of the root (same name, same value, but sword).

*Playing the game \- Defeated*  
Here, we will print all the monsters the player defeated, according to the user request, similar to the Bag:   
`=== DEFEATED MONSTERS ===`  
`1.Preorder 2.Inorder 3.Postorder`  
And according to the user choice, we will print the Defeated monsters.  
Because we have less control about what we are entering into this tree, you can assume that we will not have the exact same monster.

How is this tree ordered?  
1\. name (strcmp)  
2\. If names equal: attack  
3\. If attacks equal: hp  
4\. If hp equal: type (PHANTOM=0 \< SPIDER=1 \< DEMON=2 \< GOLEM=3 \< COBRA=4)

For example:  
`[Goblin] Type: Phantom, Attack: 3, HP: 20`  
	`[Demon] Type: Demon, Attack: 5, HP: 30`  
	`[Cobra] Type: Cobra, Attack: 8, HP: 15`  
	`[Spider] Type: Spider, Attack: 4, HP: 25`  
	`[Golem] Type: Golem, Attack: 6, HP: 40`

This is a preorder print, when Goblin is the root, Demon and Cobra are in the left subtree, and Spider and Golem are in the right subtree.

*Playing the game \- Quit*  
The quit here is not quitting the program, just go back to the first menu.  
The catch is, we can go back, and add more rooms in the middle of the game.  
And when we go back and play the game, the game will be updated with the new rooms.  
---

Winning the game  
The player will win the game, if he visits all rooms, and there are no monsters in any rooms.  
Note, there can still be items, but no monsters.  
As soon as the conditions are met, we will print:  
`***************************************`  
             `VICTORY!`                    
 `All rooms explored. All monsters defeated.`   
`***************************************`  
and exit the program.  
**Please note, no matter if we die or win \- we must free all memory we allocated.**  
---

# **Complete code example:**

Send the files you got from the git as is to the submit, and you will get the best example for input and output.