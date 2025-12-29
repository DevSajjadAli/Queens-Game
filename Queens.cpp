#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

const int BOARD_SIZE = 8;

const string RESET = "\033[0m";
const string BOLD = "\033[1m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string WHITE = "\033[37m";
const string BLACK = "\033[30m";
const string BG_RED = "\033[48;5;196m";
const string BG_GREEN = "\033[48;5;46m";
const string BG_YELLOW = "\033[48;5;226m";
const string BG_BLUE = "\033[48;5;21m";
const string BG_MAGENTA = "\033[48;5;201m";
const string BG_CYAN = "\033[48;5;51m";
const string BG_WHITE = "\033[48;5;231m";
const string BG_BRIGHT_RED = "\033[48;5;203m";
const string BG_BRIGHT_GREEN = "\033[48;5;118m";
const string BG_BRIGHT_YELLOW = "\033[48;5;228m";
const string BG_BRIGHT_BLUE = "\033[48;5;39m";
const string BG_BRIGHT_MAGENTA = "\033[48;5;213m";
const string BG_BRIGHT_CYAN = "\033[48;5;87m";
const string BG_BRIGHT_WHITE = "\033[48;5;255m";
const string BG_ORANGE = "\033[48;5;208m";
const string BG_PURPLE = "\033[48;5;129m";
const string BG_PINK = "\033[48;5;205m";
const string BG_LIME = "\033[48;5;154m";
const string BG_TEAL = "\033[48;5;30m";
const string BG_BROWN = "\033[48;5;130m";

struct MoveNode {
	int row;
	int col;
	int actionType;
	MoveNode* next;
};

class MoveHistory {
private:
	MoveNode* head;
	int count;
public:
	MoveHistory() {
		head = NULL;
		count = 0;
	}

	~MoveHistory() {
		clear();
	}

	void addMove(int row, int col, int actionType) {
		MoveNode* newNode = new MoveNode;
		newNode->row = row;
		newNode->col = col;
		newNode->actionType = actionType;
		newNode->next = head;
		head = newNode;
		count++;
	}

	MoveNode* getLastMove() {
		return head;
	}

	void removeLastMove() {
		if (head == NULL) return;
		MoveNode* temp = head;
		head = head->next;
		delete temp;
		count--;
	}

	int getCount() {
		return count;
	}

	void clear() {
		while (head != NULL) {
			MoveNode* temp = head;
			head = head->next;
			delete temp;
		}
		count = 0;
	}

	void display() {
		cout << CYAN << "Move History (" << count << " moves):\n" << RESET;
		MoveNode* temp = head;
		int i = count;
		while (temp != NULL) {
			string action;
			if (temp->actionType == 1) action = "Place Queen";
			else if (temp->actionType == 2) action = "Remove Queen";
			else if (temp->actionType == 3) action = "Mark X";
			else action = "Clear Cell";
			cout << "  " << i << ". " << action << " at (" << temp->row << ", " << temp->col << ")\n";
			temp = temp->next;
			i--;
		}
	}
};

struct UndoNode {
	int row;
	int col;
	int prevState;
	int newState;
	UndoNode* prev;
	UndoNode* next;
};

class UndoRedoList {
private:
	UndoNode* head;
	UndoNode* tail;
	UndoNode* current;
public:
	UndoRedoList() {
		head = NULL;
		tail = NULL;
		current = NULL;
	}

	~UndoRedoList() {
		clear();
	}

	void addAction(int row, int col, int prevState, int newState) {
		if (current != NULL && current->next != NULL) {
			UndoNode* temp = current->next;
			while (temp != NULL) {
				UndoNode* toDelete = temp;
				temp = temp->next;
				delete toDelete;
			}
			current->next = NULL;
			tail = current;
		}

		UndoNode* newNode = new UndoNode;
		newNode->row = row;
		newNode->col = col;
		newNode->prevState = prevState;
		newNode->newState = newState;
		newNode->prev = tail;
		newNode->next = NULL;

		if (tail != NULL) {
			tail->next = newNode;
		}
		tail = newNode;
		current = newNode;

		if (head == NULL) {
			head = newNode;
		}
	}

	UndoNode* undo() {
		if (current == NULL) return NULL;
		UndoNode* action = current;
		current = current->prev;
		return action;
	}

	UndoNode* redo() {
		if (current == NULL && head != NULL) {
			current = head;
			return current;
		}
		if (current != NULL && current->next != NULL) {
			current = current->next;
			return current;
		}
		return NULL;
	}

	bool canUndo() {
		return current != NULL;
	}

	bool canRedo() {
		return (current == NULL && head != NULL) || (current != NULL && current->next != NULL);
	}

	void clear() {
		UndoNode* temp = head;
		while (temp != NULL) {
			UndoNode* toDelete = temp;
			temp = temp->next;
			delete toDelete;
		}
		head = NULL;
		tail = NULL;
		current = NULL;
	}
};

struct MenuNode {
	int id;
	string label;
	MenuNode* next;
};

class CircularMenu {
private:
	MenuNode* head;
	MenuNode* current;
	int size;
public:
	CircularMenu() {
		head = NULL;
		current = NULL;
		size = 0;
	}

	~CircularMenu() {
		if (head == NULL) return;
		MenuNode* temp = head->next;
		while (temp != head) {
			MenuNode* toDelete = temp;
			temp = temp->next;
			delete toDelete;
		}
		delete head;
	}

	void addOption(int id, string label) {
		MenuNode* newNode = new MenuNode;
		newNode->id = id;
		newNode->label = label;

		if (head == NULL) {
			head = newNode;
			newNode->next = newNode;
			current = newNode;
		}
		else {
			MenuNode* temp = head;
			while (temp->next != head) {
				temp = temp->next;
			}
			temp->next = newNode;
			newNode->next = head;
		}
		size++;
	}

	void display() {
		cout << "\n" << CYAN << BOLD << "========== Queens Puzzle ==========" << RESET << "\n";
		if (head == NULL) return;
		MenuNode* temp = head;
		do {
			if (temp == current) {
				cout << YELLOW << BOLD << " > " << temp->id << ". " << temp->label << " <" << RESET << "\n";
			}
			else {
				cout << WHITE << "   " << temp->id << ". " << temp->label << RESET << "\n";
			}
			temp = temp->next;
		} while (temp != head);
		cout << CYAN << "===================================" << RESET << "\n";
	}

	void moveNext() {
		if (current != NULL) {
			current = current->next;
		}
	}

	void movePrev() {
		if (current == NULL || head == NULL) return;
		MenuNode* temp = head;
		while (temp->next != current) {
			temp = temp->next;
		}
		current = temp;
	}

	int getCurrentId() {
		if (current != NULL) return current->id;
		return -1;
	}

	MenuNode* getById(int id) {
		if (head == NULL) return NULL;
		MenuNode* temp = head;
		do {
			if (temp->id == id) return temp;
			temp = temp->next;
		} while (temp != head);
		return NULL;
	}
};

struct RecordNode {
	int moveCount;
	int gameId;
	bool won;
	RecordNode* left;
	RecordNode* right;
};

class GameRecordsBST {
private:
	RecordNode* root;
	int gameCounter;

	RecordNode* insert(RecordNode* node, int moves, int id, bool won) {
		if (node == NULL) {
			RecordNode* newNode = new RecordNode;
			newNode->moveCount = moves;
			newNode->gameId = id;
			newNode->won = won;
			newNode->left = NULL;
			newNode->right = NULL;
			return newNode;
		}
		if (moves < node->moveCount) {
			node->left = insert(node->left, moves, id, won);
		}
		else {
			node->right = insert(node->right, moves, id, won);
		}
		return node;
	}

	void inorder(RecordNode* node) {
		if (node == NULL) return;
		inorder(node->left);
		string status = node->won ? GREEN + "Won" + RESET : RED + "Lost" + RESET;
		cout << "  Game #" << node->gameId << ": " << node->moveCount << " moves - " << status << "\n";
		inorder(node->right);
	}

	RecordNode* findMin(RecordNode* node) {
		while (node != NULL && node->left != NULL) {
			node = node->left;
		}
		return node;
	}

	void destroyTree(RecordNode* node) {
		if (node == NULL) return;
		destroyTree(node->left);
		destroyTree(node->right);
		delete node;
	}

public:
	GameRecordsBST() {
		root = NULL;
		gameCounter = 0;
	}

	~GameRecordsBST() {
		destroyTree(root);
	}

	void addRecord(int moves, bool won) {
		gameCounter++;
		root = insert(root, moves, gameCounter, won);
	}

	void displayRecords() {
		cout << CYAN << "\n=== Game Records (sorted by moves) ===\n" << RESET;
		if (root == NULL) {
			cout << "  No games played yet.\n";
			return;
		}
		inorder(root);
	}

	RecordNode* getBestGame() {
		return findMin(root);
	}

	int getTotalGames() {
		return gameCounter;
	}
};

class ConflictGraph {
private:
	int rowConflicts[BOARD_SIZE];
	int colConflicts[BOARD_SIZE];
	int colorConflicts[BOARD_SIZE];
	int colorGrid[BOARD_SIZE][BOARD_SIZE];

public:
	ConflictGraph() {
		for (int i = 0; i < BOARD_SIZE; i++) {
			rowConflicts[i] = 0;
			colConflicts[i] = 0;
			colorConflicts[i] = 0;
		}
	}

	void setColorGrid(int grid[BOARD_SIZE][BOARD_SIZE]) {
		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				colorGrid[i][j] = grid[i][j];
			}
		}
	}

	void addQueen(int row, int col) {
		rowConflicts[row]++;
		colConflicts[col]++;
		colorConflicts[colorGrid[row][col]]++;
	}

	void removeQueen(int row, int col) {
		rowConflicts[row]--;
		colConflicts[col]--;
		colorConflicts[colorGrid[row][col]]--;
	}

	bool hasRowConflict(int row) {
		return rowConflicts[row] > 0;
	}

	bool hasColConflict(int col) {
		return colConflicts[col] > 0;
	}

	bool hasColorConflict(int row, int col) {
		return colorConflicts[colorGrid[row][col]] > 0;
	}

	int getRowCount(int row) { return rowConflicts[row]; }
	int getColCount(int col) { return colConflicts[col]; }
	int getColorCount(int color) { return colorConflicts[color]; }

	void reset() {
		for (int i = 0; i < BOARD_SIZE; i++) {
			rowConflicts[i] = 0;
			colConflicts[i] = 0;
			colorConflicts[i] = 0;
		}
	}
};

class QueensGame {
private:
	int board[BOARD_SIZE][BOARD_SIZE];
	int colorGrid[BOARD_SIZE][BOARD_SIZE];
	int queenCount;
	int moveCount;
	MoveHistory history;
	UndoRedoList undoRedo;
	ConflictGraph conflicts;
	GameRecordsBST* records;

	string regionColors[8] = {
		BG_RED, BG_LIME, BG_YELLOW, BG_BLUE,
		BG_PURPLE, BG_CYAN, BG_ORANGE, BG_PINK
	};

	string regionTextColors[8] = {
		WHITE, BLACK, BLACK, WHITE,
		WHITE, BLACK, BLACK, WHITE
	};

public:
	QueensGame(GameRecordsBST* rec) {
		srand((unsigned int)time(NULL));
		records = rec;
		queenCount = 0;
		moveCount = 0;
		initBoard();
		generateColorRegions();
		conflicts.setColorGrid(colorGrid);
	}

	void initBoard() {
		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				board[i][j] = 0;
			}
		}
		queenCount = 0;
		moveCount = 0;
		history.clear();
		undoRedo.clear();
		conflicts.reset();
	}

	void generateColorRegions() {
		int predefinedBoards[5][8][8] = {
			{
				{0, 0, 0, 0, 1, 1, 1, 1},
				{0, 2, 2, 0, 1, 1, 1, 1},
				{2, 2, 2, 0, 3, 3, 1, 1},
				{2, 2, 0, 0, 3, 3, 3, 4},
				{2, 0, 0, 3, 3, 3, 4, 4},
				{5, 5, 0, 3, 6, 6, 4, 4},
				{5, 5, 5, 6, 6, 6, 4, 4},
				{5, 5, 5, 6, 6, 7, 7, 7}
			},
			{
				{0, 0, 0, 1, 1, 1, 1, 2},
				{0, 0, 1, 1, 1, 2, 2, 2},
				{0, 3, 3, 1, 2, 2, 2, 2},
				{3, 3, 3, 3, 4, 4, 2, 5},
				{3, 3, 4, 4, 4, 4, 5, 5},
				{6, 6, 4, 4, 5, 5, 5, 5},
				{6, 6, 6, 7, 7, 5, 5, 5},
				{6, 6, 6, 7, 7, 7, 7, 7}
			},
			{
				{0, 0, 0, 0, 1, 1, 1, 1},
				{0, 0, 2, 2, 2, 1, 1, 1},
				{0, 2, 2, 2, 3, 3, 1, 1},
				{0, 2, 2, 3, 3, 3, 3, 4},
				{5, 5, 2, 3, 3, 4, 4, 4},
				{5, 5, 5, 5, 6, 6, 4, 4},
				{5, 5, 7, 6, 6, 6, 4, 4},
				{7, 7, 7, 7, 6, 6, 6, 4}
			},
			{
				{0, 0, 1, 1, 1, 2, 2, 2},
				{0, 0, 0, 1, 1, 1, 2, 2},
				{0, 0, 3, 3, 1, 2, 2, 2},
				{4, 4, 3, 3, 3, 3, 5, 5},
				{4, 4, 4, 3, 3, 5, 5, 5},
				{4, 4, 4, 6, 6, 6, 5, 5},
				{7, 4, 6, 6, 6, 6, 5, 5},
				{7, 7, 7, 7, 6, 6, 5, 5}
			},
			{
				{0, 0, 0, 0, 0, 1, 1, 1},
				{0, 2, 2, 3, 3, 3, 1, 1},
				{2, 2, 2, 3, 3, 3, 1, 1},
				{2, 2, 3, 3, 4, 4, 4, 1},
				{2, 5, 5, 4, 4, 4, 4, 1},
				{5, 5, 5, 5, 4, 6, 6, 6},
				{5, 5, 7, 7, 6, 6, 6, 6},
				{7, 7, 7, 7, 7, 7, 6, 6}
			}
		};

		int boardIndex = rand() % 5;

		int shuffle[8];
		for (int i = 0; i < 8; i++) shuffle[i] = i;
		for (int i = 7; i > 0; i--) {
			int j = rand() % (i + 1);
			int temp = shuffle[i];
			shuffle[i] = shuffle[j];
			shuffle[j] = temp;
		}

		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				colorGrid[i][j] = shuffle[predefinedBoards[boardIndex][i][j]];
			}
		}

		int transform = rand() % 8;
		if (transform >= 4) {
			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = i + 1; j < BOARD_SIZE; j++) {
					int temp = colorGrid[i][j];
					colorGrid[i][j] = colorGrid[j][i];
					colorGrid[j][i] = temp;
				}
			}
		}
		if (transform % 4 >= 2) {
			for (int i = 0; i < BOARD_SIZE / 2; i++) {
				for (int j = 0; j < BOARD_SIZE; j++) {
					int temp = colorGrid[i][j];
					colorGrid[i][j] = colorGrid[BOARD_SIZE - 1 - i][j];
					colorGrid[BOARD_SIZE - 1 - i][j] = temp;
				}
			}
		}
		if (transform % 2 == 1) {
			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = 0; j < BOARD_SIZE / 2; j++) {
					int temp = colorGrid[i][j];
					colorGrid[i][j] = colorGrid[i][BOARD_SIZE - 1 - j];
					colorGrid[i][BOARD_SIZE - 1 - j] = temp;
				}
			}
		}

		conflicts.setColorGrid(colorGrid);
	}

	void displayBoard() {
		cout << "\n";

		cout << "      ";
		for (int j = 0; j < BOARD_SIZE; j++) {
			cout << CYAN << BOLD << " " << j << "  " << RESET;
		}
		cout << "\n";

		cout << "     " << CYAN << "+";
		for (int j = 0; j < BOARD_SIZE; j++) {
			cout << "---+";
		}
		cout << RESET << "\n";

		for (int i = 0; i < BOARD_SIZE; i++) {
			cout << CYAN << BOLD << "  " << i << "  " << RESET << CYAN << "|" << RESET;

			for (int j = 0; j < BOARD_SIZE; j++) {
				int c = colorGrid[i][j];
				string bg = regionColors[c];
				string fg = regionTextColors[c];
				string content;

				if (board[i][j] == 1) {
					content = fg + BOLD + " Q " + RESET;
				}
				else if (board[i][j] == 2) {
					content = fg + " X " + RESET;
				}
				else {
					content = fg + " . " + RESET;
				}

				cout << bg << content << RESET << CYAN << "|" << RESET;
			}
			cout << "\n";

			cout << "     " << CYAN << "+";
			for (int j = 0; j < BOARD_SIZE; j++) {
				cout << "---+";
			}
			cout << RESET << "\n";
		}

		cout << "\n" << YELLOW << "Queens: " << queenCount << "/8" << RESET;
		cout << "  |  " << CYAN << "Moves: " << moveCount << RESET << "\n";

		cout << "\n" << WHITE << "Regions: " << RESET;
		for (int c = 0; c < BOARD_SIZE; c++) {
			int cnt = 0;
			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = 0; j < BOARD_SIZE; j++) {
					if (colorGrid[i][j] == c && board[i][j] == 1) cnt++;
				}
			}
			cout << regionColors[c] << regionTextColors[c] << " " << c << ":" << cnt << "/1 " << RESET << " ";
		}
		cout << "\n";
	}

	bool isValidPosition(int row, int col) {
		return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
	}

	bool hasDiagonalTouch(int row, int col) {
		int dr[] = { -1, -1, 1, 1 };
		int dc[] = { -1, 1, -1, 1 };

		for (int d = 0; d < 4; d++) {
			int nr = row + dr[d];
			int nc = col + dc[d];
			if (isValidPosition(nr, nc) && board[nr][nc] == 1) {
				return true;
			}
		}
		return false;
	}

	bool canPlaceQueen(int row, int col) {
		if (!isValidPosition(row, col)) return false;
		if (board[row][col] != 0) return false;
		if (conflicts.hasRowConflict(row)) return false;
		if (conflicts.hasColConflict(col)) return false;
		if (conflicts.hasColorConflict(row, col)) return false;
		if (hasDiagonalTouch(row, col)) return false;
		return true;
	}

	void recalculateInvalidMarks() {
		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == 2) {
					board[i][j] = 0;
				}
			}
		}

		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == 0 && !canPlaceQueen(i, j)) {
					board[i][j] = 2;
				}
			}
		}
	}

	bool placeQueen(int row, int col) {
		if (!isValidPosition(row, col)) {
			cout << RED << "Invalid position! Use 0-7 for row and column.\n" << RESET;
			return false;
		}

		if (board[row][col] == 1) {
			cout << RED << "There's already a queen here!\n" << RESET;
			return false;
		}

		if (board[row][col] == 2) {
			cout << RED << "This cell is marked as invalid. Clear it first or choose another.\n" << RESET;
			return false;
		}

		if (!canPlaceQueen(row, col)) {
			if (conflicts.hasRowConflict(row)) {
				cout << RED << "Invalid! Row " << row << " already has a queen.\n" << RESET;
			}
			else if (conflicts.hasColConflict(col)) {
				cout << RED << "Invalid! Column " << col << " already has a queen.\n" << RESET;
			}
			else if (conflicts.hasColorConflict(row, col)) {
				cout << RED << "Invalid! This color region already has a queen.\n" << RESET;
			}
			else if (hasDiagonalTouch(row, col)) {
				cout << RED << "Invalid! Queens cannot touch diagonally.\n" << RESET;
			}
			return false;
		}

		int prevState = board[row][col];
		board[row][col] = 1;
		queenCount++;
		moveCount++;

		conflicts.addQueen(row, col);
		history.addMove(row, col, 1);
		undoRedo.addAction(row, col, prevState, 1);

		recalculateInvalidMarks();

		cout << GREEN << "Queen placed at (" << row << ", " << col << ")!\n" << RESET;
		return true;
	}

	bool removeQueen(int row, int col) {
		if (!isValidPosition(row, col)) {
			cout << RED << "Invalid position!\n" << RESET;
			return false;
		}

		if (board[row][col] != 1) {
			cout << RED << "No queen at this position!\n" << RESET;
			return false;
		}

		int prevState = board[row][col];
		board[row][col] = 0;
		queenCount--;
		moveCount++;

		conflicts.removeQueen(row, col);
		history.addMove(row, col, 2);
		undoRedo.addAction(row, col, prevState, 0);

		recalculateInvalidMarks();

		cout << GREEN << "Queen removed from (" << row << ", " << col << ")!\n" << RESET;
		return true;
	}

	bool markX(int row, int col) {
		if (!isValidPosition(row, col)) {
			cout << RED << "Invalid position!\n" << RESET;
			return false;
		}

		if (board[row][col] == 1) {
			cout << RED << "Cannot mark a queen position!\n" << RESET;
			return false;
		}

		if (board[row][col] == 2) {
			cout << YELLOW << "Already marked as X.\n" << RESET;
			return false;
		}

		int prevState = board[row][col];
		board[row][col] = 2;
		moveCount++;

		history.addMove(row, col, 3);
		undoRedo.addAction(row, col, prevState, 2);

		cout << GREEN << "Marked X at (" << row << ", " << col << ").\n" << RESET;
		return true;
	}

	bool clearCell(int row, int col) {
		if (!isValidPosition(row, col)) {
			cout << RED << "Invalid position!\n" << RESET;
			return false;
		}

		if (board[row][col] == 0) {
			cout << YELLOW << "Cell is already empty.\n" << RESET;
			return false;
		}

		int prevState = board[row][col];

		if (board[row][col] == 1) {
			conflicts.removeQueen(row, col);
			queenCount--;
		}

		board[row][col] = 0;
		moveCount++;

		history.addMove(row, col, 4);
		undoRedo.addAction(row, col, prevState, 0);

		recalculateInvalidMarks();

		cout << GREEN << "Cell cleared at (" << row << ", " << col << ").\n" << RESET;
		return true;
	}

	bool undo() {
		if (!undoRedo.canUndo()) {
			cout << RED << "Nothing to undo!\n" << RESET;
			return false;
		}

		UndoNode* action = undoRedo.undo();
		if (action == NULL) return false;

		if (action->newState == 1) {
			conflicts.removeQueen(action->row, action->col);
			queenCount--;
		}
		if (action->prevState == 1) {
			conflicts.addQueen(action->row, action->col);
			queenCount++;
		}

		board[action->row][action->col] = action->prevState;
		recalculateInvalidMarks();

		cout << GREEN << "Undo successful!\n" << RESET;
		return true;
	}

	bool redo() {
		if (!undoRedo.canRedo()) {
			cout << RED << "Nothing to redo!\n" << RESET;
			return false;
		}

		UndoNode* action = undoRedo.redo();
		if (action == NULL) return false;

		if (action->prevState == 1) {
			conflicts.removeQueen(action->row, action->col);
			queenCount--;
		}
		if (action->newState == 1) {
			conflicts.addQueen(action->row, action->col);
			queenCount++;
		}

		board[action->row][action->col] = action->newState;
		recalculateInvalidMarks();

		cout << GREEN << "Redo successful!\n" << RESET;
		return true;
	}

	void showHint() {
		if (queenCount >= 8) {
			cout << YELLOW << "Puzzle already solved!\n" << RESET;
			return;
		}

		int bestRow = -1, bestCol = -1;
		int minBlockedCells = BOARD_SIZE * BOARD_SIZE + 1;
		string bestReason = "";

		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == 0 && canPlaceQueen(i, j)) {
					int blocked = 0;
					string reason = "";

					int rowEmpty = 0;
					for (int c = 0; c < BOARD_SIZE; c++) {
						if (c != j && board[i][c] == 0) rowEmpty++;
					}

					int colEmpty = 0;
					for (int r = 0; r < BOARD_SIZE; r++) {
						if (r != i && board[r][j] == 0) colEmpty++;
					}

					int colorEmpty = 0;
					int color = colorGrid[i][j];
					for (int r = 0; r < BOARD_SIZE; r++) {
						for (int c = 0; c < BOARD_SIZE; c++) {
							if ((r != i || c != j) && colorGrid[r][c] == color && board[r][c] == 0) {
								colorEmpty++;
							}
						}
					}

					blocked = rowEmpty + colEmpty + colorEmpty;

					int dr[] = { -1, -1, 1, 1 };
					int dc[] = { -1, 1, -1, 1 };
					for (int d = 0; d < 4; d++) {
						int nr = i + dr[d];
						int nc = j + dc[d];
						if (isValidPosition(nr, nc) && board[nr][nc] == 0) {
							blocked++;
						}
					}

					if (blocked < minBlockedCells) {
						minBlockedCells = blocked;
						bestRow = i;
						bestCol = j;

						if (rowEmpty <= 1) {
							reason = "This row has limited options.";
						}
						else if (colEmpty <= 1) {
							reason = "This column has limited options.";
						}
						else if (colorEmpty <= 1) {
							reason = "This color region has limited options.";
						}
						else {
							reason = "This spot keeps more cells open for future moves.";
						}
						bestReason = reason;
					}
				}
			}
		}

		if (bestRow == -1) {
			cout << RED << "No valid moves available! Try undoing some moves.\n" << RESET;
			return;
		}

		cout << "\n" << CYAN << BOLD << "=== HINT ===" << RESET << "\n";
		cout << GREEN << "Suggested move: (" << bestRow << ", " << bestCol << ")\n" << RESET;
		cout << WHITE << "Reason: " << bestReason << "\n" << RESET;
		cout << YELLOW << "(Queen not placed - make the move yourself!)\n" << RESET;
	}

	bool checkWin() {
		return queenCount == 8;
	}

	void restart() {
		if (moveCount > 0) {
			records->addRecord(moveCount, checkWin());
		}

		initBoard();
		generateColorRegions();
		conflicts.setColorGrid(colorGrid);

		cout << GREEN << "\n*** New Game Started! ***\n" << RESET;
	}

	int getMoveCount() {
		return moveCount;
	}

	void showHistory() {
		history.display();
	}
};

int main() {
	GameRecordsBST records;

	CircularMenu menu;
	menu.addOption(1, "Place Queen");
	menu.addOption(2, "Remove Queen");
	menu.addOption(3, "Mark X");
	menu.addOption(4, "Clear Cell");
	menu.addOption(5, "Get Hint");
	menu.addOption(6, "Undo");
	menu.addOption(7, "Redo");
	menu.addOption(8, "Show History");
	menu.addOption(9, "Restart Game");
	menu.addOption(10, "View Records");
	menu.addOption(11, "Exit");

	QueensGame game(&records);

	cout << GREEN << BOLD << "\n";
	cout << "  ____                              ____                 _      \n";
	cout << " / __ \\                            |  _ \\               | |     \n";
	cout << "| |  | |_   _  ___  ___ _ __  ___  | |_) |_   _ _______ | | ___ \n";
	cout << "| |  | | | | |/ _ \\/ _ \\ '_ \\/ __| |  __/| | | |_  /_  /| |/ _ \\\n";
	cout << "| |__| | |_| |  __/  __/ | | \\__ \\ | |   | |_| |/ / / /_| |  __/\n";
	cout << " \\___\\_\\\\__,_|\\___|\\___/_| |_|___/ |_|    \\__,_/___/_____|_|\\___|\n";
	cout << RESET << "\n";

	cout << WHITE << "Welcome to the Queens Puzzle Game!\n\n" << RESET;
	cout << CYAN << "Rules:\n" << RESET;
	cout << "  - Place exactly " << BOLD << "one queen per row" << RESET << "\n";
	cout << "  - Place exactly " << BOLD << "one queen per column" << RESET << "\n";
	cout << "  - Place exactly " << BOLD << "one queen per color region" << RESET << "\n";
	cout << "  - Queens " << BOLD << "cannot touch diagonally" << RESET << "\n";
	cout << "\n" << WHITE << "Legend: " << BOLD << "Q" << RESET << " = Queen, ";
	cout << RED << "X" << RESET << " = Invalid, " << WHITE << "." << RESET << " = Empty\n";

	game.displayBoard();

	bool playing = true;
	int choice, row, col;

	while (playing) {
		menu.display();
		cout << "Enter choice (1-11): ";
		cin >> choice;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(10000, '\n');
			cout << RED << "Invalid input! Please enter a number.\n" << RESET;
			continue;
		}

		switch (choice) {
		case 1:
			cout << "Enter row (0-7): ";
			cin >> row;
			cout << "Enter column (0-7): ";
			cin >> col;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << RED << "Invalid input!\n" << RESET;
				break;
			}
			game.placeQueen(row, col);
			game.displayBoard();
			if (game.checkWin()) {
				cout << "\n" << GREEN << BOLD;
				cout << "**********************************************\n";
				cout << "*                                            *\n";
				cout << "*   CONGRATULATIONS! You solved the puzzle!  *\n";
				cout << "*   All 8 queens placed successfully!        *\n";
				cout << "*                                            *\n";
				cout << "**********************************************\n";
				cout << RESET;
				cout << YELLOW << "Total moves: " << game.getMoveCount() << "\n" << RESET;
				records.addRecord(game.getMoveCount(), true);
			}
			break;

		case 2:
			cout << "Enter row (0-7): ";
			cin >> row;
			cout << "Enter column (0-7): ";
			cin >> col;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << RED << "Invalid input!\n" << RESET;
				break;
			}
			game.removeQueen(row, col);
			game.displayBoard();
			break;

		case 3:
			cout << "Enter row (0-7): ";
			cin >> row;
			cout << "Enter column (0-7): ";
			cin >> col;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << RED << "Invalid input!\n" << RESET;
				break;
			}
			game.markX(row, col);
			game.displayBoard();
			break;

		case 4:
			cout << "Enter row (0-7): ";
			cin >> row;
			cout << "Enter column (0-7): ";
			cin >> col;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << RED << "Invalid input!\n" << RESET;
				break;
			}
			game.clearCell(row, col);
			game.displayBoard();
			break;

		case 5:
			game.showHint();
			break;

		case 6:
			game.undo();
			game.displayBoard();
			break;

		case 7:
			game.redo();
			game.displayBoard();
			break;

		case 8:
			game.showHistory();
			break;

		case 9:
			game.restart();
			game.displayBoard();
			break;

		case 10:
			records.displayRecords();
			break;

		case 11:
			playing = false;
			if (game.getMoveCount() > 0 && !game.checkWin()) {
				records.addRecord(game.getMoveCount(), false);
			}
			cout << GREEN << "\nThanks for playing! Goodbye!\n" << RESET;
			break;

		default:
			cout << RED << "Invalid choice! Please enter 1-11.\n" << RESET;
			break;
		}
	}

	return 0;
}
