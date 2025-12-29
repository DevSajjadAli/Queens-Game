# Queens Puzzle Game - Complete Project Documentation

## 📋 Project Overview

This is a **Console-based Queens Puzzle Game** implemented in C++ that demonstrates multiple **Data Structure and Algorithm (DSA)** concepts. The game is inspired by the LinkedIn Queens Game where you must place 8 queens on an 8×8 board following specific rules.

### Game Rules
1. **One queen per row** - Each row can have exactly one queen
2. **One queen per column** - Each column can have exactly one queen
3. **One queen per color region** - Each colored region can have exactly one queen
4. **No diagonal touching** - Queens cannot be adjacent diagonally (touching corners)

> **Note:** This is different from traditional 8-Queens chess problem where queens attack along entire diagonals.

---

## 🗂️ Data Structures Used

### 1. Singly Linked List (`MoveHistory`)
**Purpose:** Tracks all moves made during the game

**Structure:**
```
MoveNode:
├── row (int)        - Row position of move
├── col (int)        - Column position of move
├── actionType (int) - 1=Place, 2=Remove, 3=MarkX, 4=Clear
└── next (MoveNode*) - Pointer to next node
```

**Operations:**
| Method | Time Complexity | Description |
|--------|----------------|-------------|
| `addMove()` | O(1) | Insert at head (newest move first) |
| `getLastMove()` | O(1) | Return head pointer |
| `removeLastMove()` | O(1) | Delete head node |
| `display()` | O(n) | Traverse and print all moves |
| `clear()` | O(n) | Delete all nodes |

**Why Singly Linked List?**
- Stack-like behavior (LIFO - Last In First Out)
- Only need access to most recent move
- Dynamic size - no need to predict number of moves

---

### 2. Doubly Linked List (`UndoRedoList`)
**Purpose:** Enables undo and redo functionality

**Structure:**
```
UndoNode:
├── row (int)        - Row position
├── col (int)        - Column position
├── prevState (int)  - State before action (0=empty, 1=queen, 2=X)
├── newState (int)   - State after action
├── prev (UndoNode*) - Pointer to previous action
└── next (UndoNode*) - Pointer to next action
```

**Key Variables:**
- `head` - First action in history
- `tail` - Last action added
- `current` - Current position in undo/redo timeline

**Operations:**
| Method | Time Complexity | Description |
|--------|----------------|-------------|
| `addAction()` | O(1) | Add new action, clear future if exists |
| `undo()` | O(1) | Move current pointer backward |
| `redo()` | O(1) | Move current pointer forward |
| `canUndo()` | O(1) | Check if undo is possible |
| `canRedo()` | O(1) | Check if redo is possible |

**Why Doubly Linked List?**
- Need bidirectional traversal (undo goes back, redo goes forward)
- When new action after undo, discard all "future" actions
- Each node stores both previous and new state for reversal

**Undo/Redo Concept:**
```
Actions: [A] ↔ [B] ↔ [C] ↔ [D]
                           ↑
                        current

After 2 undos:
Actions: [A] ↔ [B] ↔ [C] ↔ [D]
               ↑
            current

After new action [E]:
Actions: [A] ↔ [B] ↔ [E]    (C and D deleted)
                     ↑
                  current
```

---

### 3. Circular Linked List (`CircularMenu`)
**Purpose:** Implements the game menu system

**Structure:**
```
MenuNode:
├── id (int)         - Menu option number (1-11)
├── label (string)   - Display text
└── next (MenuNode*) - Pointer to next option (last→first)
```

**Circular Nature:**
```
[1.Place] → [2.Remove] → [3.MarkX] → ... → [11.Exit] ─┐
    ↑___________________________________________________│
```

**Operations:**
| Method | Time Complexity | Description |
|--------|----------------|-------------|
| `addOption()` | O(n) | Add to end, maintain circularity |
| `display()` | O(n) | Print all options, highlight current |
| `moveNext()` | O(1) | Move to next option |
| `movePrev()` | O(n) | Move to previous option |
| `getById()` | O(n) | Find option by ID |

**Why Circular Linked List?**
- Menu wraps around (after last option, go to first)
- Natural representation of cyclic menu navigation
- Demonstrates circular structure concept

---

### 4. Binary Search Tree (`GameRecordsBST`)
**Purpose:** Stores game records sorted by number of moves

**Structure:**
```
RecordNode:
├── moveCount (int)   - Number of moves in game
├── gameId (int)      - Sequential game number
├── won (bool)        - Whether game was won
├── left (RecordNode*) - Games with fewer moves
└── right (RecordNode*) - Games with more moves
```

**Tree Example:**
```
                 [15 moves]
                /          \
        [10 moves]      [20 moves]
        /      \            \
   [8 moves] [12 moves]  [25 moves]
```

**Operations:**
| Method | Time Complexity | Description |
|--------|----------------|-------------|
| `insert()` | O(log n) avg, O(n) worst | Add new record |
| `inorder()` | O(n) | Display sorted by moves |
| `findMin()` | O(log n) | Find best (fewest moves) game |
| `addRecord()` | O(log n) | Public insert wrapper |

**Why BST?**
- Automatic sorting by move count
- Efficient retrieval of best game (minimum)
- In-order traversal gives sorted display

---

### 5. Graph (`ConflictGraph`)
**Purpose:** Tracks constraints (row, column, color region conflicts)

**Implementation:** Adjacency-like arrays tracking queen counts

**Structure:**
```cpp
int rowConflicts[8];    // Queens in each row
int colConflicts[8];    // Queens in each column
int colorConflicts[8];  // Queens in each color region
int colorGrid[8][8];    // Maps cell to color
```

**Concept:**
- Graph nodes = Rows, Columns, Color Regions
- Graph edges = Cells connecting to their row, column, color
- Conflict = More than one queen in same row/column/color

```
     Rows      Columns    Colors
     [0]         [0]        [Red]
     [1]         [1]        [Green]
     ...         ...        ...
     [7]         [7]        [Purple]
     
Cell (2,3) connects to: Row[2], Col[3], Color[colorGrid[2][3]]
```

**Operations:**
| Method | Time Complexity | Description |
|--------|----------------|-------------|
| `addQueen()` | O(1) | Increment all three conflict arrays |
| `removeQueen()` | O(1) | Decrement all three conflict arrays |
| `hasRowConflict()` | O(1) | Check if row has queen |
| `hasColConflict()` | O(1) | Check if column has queen |
| `hasColorConflict()` | O(1) | Check if color region has queen |

**Why Graph Concept?**
- Efficient O(1) conflict checking
- Alternative to O(n) board scanning
- Demonstrates graph representation for constraints

---

### 6. 2D Arrays (Board and Color Grid)
**Purpose:** Store board state and color region mapping

**Board Array:**
```cpp
int board[8][8];
// Values: 0 = Empty, 1 = Queen, 2 = Marked X
```

**Color Grid Array:**
```cpp
int colorGrid[8][8];
// Values: 0-7 representing 8 different color regions
```

**Visual Representation:**
```
Board:                  Color Grid:
0 0 0 1 0 0 0 0        0 0 1 1 1 2 2 2
0 0 0 0 0 1 0 0        0 0 0 1 1 1 2 2
0 0 2 0 0 0 0 0        0 0 3 3 1 2 2 2
...                     ...
```

---

## 🧩 Key Algorithms

### 1. Color Region Generation
**Method:** `generateColorRegions()`

**Algorithm:**
1. Select one of 5 predefined board patterns randomly
2. Apply Fisher-Yates shuffle to randomize color assignment
3. Apply random transformation (rotate/flip)

**Transformations (8 total):**
- Original
- Transpose (swap rows and columns)
- Horizontal flip
- Vertical flip
- Combinations of above

**Total Variations:** 5 patterns × 8! permutations × 8 transforms = **161,280 boards**

---

### 2. Queen Placement Validation
**Method:** `canPlaceQueen(int row, int col)`

```
Checks (in order):
1. Position valid (0-7 range)?
2. Cell empty (not queen or X)?
3. Row conflict? (ConflictGraph)
4. Column conflict? (ConflictGraph)
5. Color region conflict? (ConflictGraph)
6. Diagonal touch? (Check 4 diagonal neighbors)
```

**Time Complexity:** O(1) - All checks are constant time

---

### 3. Diagonal Touch Detection
**Method:** `hasDiagonalTouch(int row, int col)`

```
Check 4 diagonal neighbors:
    [r-1,c-1]       [r-1,c+1]
           \         /
            [r, c]
           /         \
    [r+1,c-1]       [r+1,c+1]
```

---

### 4. Auto-Mark Invalid Cells
**Method:** `recalculateInvalidMarks()`

After each move:
1. Clear all X marks
2. For each empty cell, check if queen can be placed
3. If not valid, mark as X

---

### 5. Hint Algorithm
**Method:** `showHint()`

**Strategy:** Find the cell that blocks the fewest future options

```
For each valid cell:
    count = empty cells in same row
          + empty cells in same column
          + empty cells in same color
          + empty diagonal neighbors
    
Suggest cell with minimum count
```

**Reasoning:** Placing queen where it blocks fewer cells keeps more options open.

---

## 📊 Time & Space Complexity Summary

| Operation | Time | Space | Data Structure |
|-----------|------|-------|----------------|
| Place Queen | O(1) | O(1) | Graph, Array |
| Check Valid | O(1) | O(1) | Graph |
| Undo/Redo | O(1) | O(n) | Doubly Linked List |
| Add to History | O(1) | O(n) | Singly Linked List |
| Add Record | O(log n) | O(n) | BST |
| Display Records | O(n) | O(1) | BST In-order |
| Generate Board | O(1) | O(1) | Arrays |
| Get Hint | O(n²) | O(1) | Brute Force |
| Display Board | O(n²) | O(1) | Arrays |

---

## 🎮 Game Features

### Menu Options
1. **Place Queen** - Put queen at specified position
2. **Remove Queen** - Take queen from position
3. **Mark X** - Mark cell as cannot place queen
4. **Clear Cell** - Remove queen or X mark
5. **Get Hint** - Suggest best next move
6. **Undo** - Reverse last action
7. **Redo** - Re-apply undone action
8. **Show History** - Display all moves made
9. **Restart Game** - Start fresh with new board
10. **View Records** - See all game statistics
11. **Exit** - Quit game

### Visual Display
- ANSI color codes for colored regions
- Grid display with borders
- Queen count and move counter
- Per-region queen status

---

## 🔑 Important Viva Questions & Answers

### Q1: Why use Singly Linked List for Move History?
**Answer:** Move history follows LIFO (Last In First Out) pattern - we mostly need the last move. Singly linked list provides O(1) insertion at head and works like a stack. We don't need backward traversal since we only display from newest to oldest.

### Q2: Why use Doubly Linked List for Undo/Redo?
**Answer:** Undo/Redo requires bidirectional traversal. Undo moves backward (prev pointer), Redo moves forward (next pointer). Doubly linked list allows O(1) movement in both directions. Also, when we make a new move after undo, we need to delete all "future" nodes.

### Q3: Why use Circular Linked List for Menu?
**Answer:** Menu naturally wraps around - after last option, we might want to go to first. Circular linked list represents this cyclic behavior naturally. The last node points back to first node, eliminating null checks for wraparound.

### Q4: Explain the Graph representation for conflicts.
**Answer:** We use three arrays representing constraints: rows, columns, and colors. Each acts like a node, and cells are edges connecting to their row, column, and color region. When placing a queen, we increment counts. Checking conflict is O(1) array access instead of O(n) board scan.

### Q5: Why use BST for game records?
**Answer:** BST automatically maintains sorted order by move count. In-order traversal gives sorted display. Finding the best game (minimum moves) is O(log n) - just follow left pointers. Insertion maintains sort order automatically.

### Q6: How does the hint algorithm work?
**Answer:** For each valid empty cell, we count how many cells it would "block" (same row, column, color region, diagonals). We suggest the cell that blocks the minimum, keeping more options open for future moves.

### Q7: What's the difference between this and traditional 8-Queens?
**Answer:** Traditional 8-Queens: Queens attack along entire rows, columns, AND diagonals (like chess).
This game: Queens only conflict in same row, column, OR color region, plus cannot touch diagonally (adjacent squares only).

### Q8: How do you ensure each game is different?
**Answer:** Three randomization layers:
1. Random selection from 5 predefined patterns
2. Fisher-Yates shuffle for color assignment
3. Random geometric transformation (flip/rotate)
Total: 5 × 40320 × 8 = 161,280 variations

### Q9: Explain the undo implementation.
**Answer:** Each action stores: position, previous state, new state. Undo retrieves current node, swaps board back to prevState, moves current pointer to prev. If it was a queen placement, we also update ConflictGraph by removing the queen from conflict arrays.

### Q10: What's the purpose of `recalculateInvalidMarks()`?
**Answer:** After any move, the valid cells change. This function clears all X marks, then re-checks every empty cell using `canPlaceQueen()`. Cells that can't accept a queen (due to row/column/color/diagonal conflicts) get marked X, helping the player visualize constraints.

---

## 📁 Code Structure

```
Queens.cpp
├── Constants & Color Definitions (Lines 1-33)
├── MoveNode struct & MoveHistory class (Lines 35-102)
│   └── Singly Linked List implementation
├── UndoNode struct & UndoRedoList class (Lines 104-202)
│   └── Doubly Linked List implementation
├── MenuNode struct & CircularMenu class (Lines 204-298)
│   └── Circular Linked List implementation
├── RecordNode struct & GameRecordsBST class (Lines 300-392)
│   └── Binary Search Tree implementation
├── ConflictGraph class (Lines 394-448)
│   └── Graph-like constraint tracking
├── QueensGame class (Lines 450-990)
│   ├── Board and ColorGrid arrays
│   ├── generateColorRegions()
│   ├── displayBoard()
│   ├── canPlaceQueen(), hasDiagonalTouch()
│   ├── placeQueen(), removeQueen(), markX(), clearCell()
│   ├── undo(), redo()
│   ├── showHint()
│   └── checkWin(), restart()
└── main() function (Lines 992-1160)
    └── Game loop and menu handling
```

---

## ✅ Learning Outcomes Demonstrated

1. **Linked Lists** - Singly, Doubly, Circular implementations
2. **Trees** - Binary Search Tree with insert, traverse, find-min
3. **Graphs** - Constraint modeling using adjacency-like arrays
4. **Arrays** - 2D array manipulation for board state
5. **Algorithms** - Search, validation, optimization (hint)
6. **OOP Concepts** - Classes, encapsulation, constructors, destructors
7. **Memory Management** - Dynamic allocation, proper deletion
8. **User Interface** - Console colors, formatted output

---

*Created for DSA Course Project - Queens Puzzle Game*
