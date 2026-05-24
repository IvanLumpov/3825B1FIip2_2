#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>

const int FIELD_SIZE = 10;      
const int SHIP_TYPES = 4;       

const int SHIP_COUNTS[SHIP_TYPES] = { 4, 3, 2, 1 };
const int SHIP_SIZES[SHIP_TYPES] = { 1, 2, 3, 4 };

enum CellState {
    CELL_EMPTY = 0,      
    CELL_SHIP = 1,       
    CELL_HIT = 2,        
    CELL_MISS = 3,       
    CELL_DESTROYED = 4   
};

struct Coord {
    int row;  
    int col;  
};


class GameField {
private:
    CellState grid[FIELD_SIZE][FIELD_SIZE];

    bool isValidCoord(int row, int col) const {
        return (row >= 0 && row < FIELD_SIZE && col >= 0 && col < FIELD_SIZE);
    }

    bool isAreaClear(int row, int col) const {
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                int r = row + dr;
                int c = col + dc;
                if (isValidCoord(r, c) && grid[r][c] != CELL_EMPTY) {
                    return false;
                }
            }
        }
        return true;
    }

public:
    GameField() {
        for (int i = 0; i < FIELD_SIZE; ++i) {
            for (int j = 0; j < FIELD_SIZE; ++j) {
                grid[i][j] = CELL_EMPTY;
            }
        }
    }

    GameField(const GameField& other) {
        for (int i = 0; i < FIELD_SIZE; ++i) {
            for (int j = 0; j < FIELD_SIZE; ++j) {
                grid[i][j] = other.grid[i][j];
            }
        }
    }

    GameField& operator=(const GameField& other) {
        if (this == &other) return *this;
        for (int i = 0; i < FIELD_SIZE; ++i) {
            for (int j = 0; j < FIELD_SIZE; ++j) {
                grid[i][j] = other.grid[i][j];
            }
        }
        return *this;
    }

    ~GameField() {}

    CellState getCell(int row, int col) const {
        if (!isValidCoord(row, col)) return CELL_EMPTY;
        return grid[row][col];
    }

    void setCell(int row, int col, CellState state) {
        if (isValidCoord(row, col)) {
            grid[row][col] = state;
        }
    }

    bool canPlaceShip(int row, int col, int size, bool isHorizontal) const {
        for (int k = 0; k < size; ++k) {
            int r = row + (isHorizontal ? 0 : k);
            int c = col + (isHorizontal ? k : 0);
            if (!isValidCoord(r, c)) return false;
            if (!isAreaClear(r, c)) return false;
        }
        return true;
    }

    void placeShip(int row, int col, int size, bool isHorizontal) {
        for (int k = 0; k < size; ++k) {
            int r = row + (isHorizontal ? 0 : k);
            int c = col + (isHorizontal ? k : 0);
            if (isValidCoord(r, c)) {
                grid[r][c] = CELL_SHIP;
            }
        }
    }

    int countAliveShips() const {
        int count = 0;
        for (int i = 0; i < FIELD_SIZE; ++i) {
            for (int j = 0; j < FIELD_SIZE; ++j) {
                if (grid[i][j] == CELL_SHIP) {
                    ++count;
                }
            }
        }
        return count;
    }

    int shoot(int row, int col) {
        if (!isValidCoord(row, col)) return 0;

        if (grid[row][col] == CELL_SHIP) {
            grid[row][col] = CELL_HIT;

            if (isShipDestroyed(row, col)) {
                markShipAndCell(row, col);
                return 2; 
            }
            return 1; 
        }

        if (grid[row][col] == CELL_EMPTY) {
            grid[row][col] = CELL_MISS;
            return 0;
        }

        return -1; 
    }

    void markShipAndCell(int row, int col) {
        int left = col;
        while (left >= 0 && grid[row][left] == CELL_HIT) --left;
        ++left;
        int right = col;
        while (right < FIELD_SIZE && grid[row][right] == CELL_HIT) ++right;
        --right;

        if (left < right) {
            for (int c = left; c <= right; ++c) {
                grid[row][c] = CELL_DESTROYED;
            }
            for (int c = left; c <= right; ++c) {
                MissNeighbors(row, c);
            }
            return;
        }

        int top = row;
        while (top >= 0 && grid[top][col] == CELL_HIT) --top;
        ++top;
        int bottom = row;
        while (bottom < FIELD_SIZE && grid[bottom][col] == CELL_HIT) ++bottom;
        --bottom;

        for (int r = top; r <= bottom; ++r) {
            grid[r][col] = CELL_DESTROYED;
        }
        for (int r = top; r <= bottom; ++r) {
            MissNeighbors(r, col);
        }
    }

    bool isAlreadyShot(int row, int col) const {
        if (!isValidCoord(row, col)) return true;
        return (grid[row][col] == CELL_HIT ||
            grid[row][col] == CELL_MISS ||
            grid[row][col] == CELL_DESTROYED);
    }

    void clear() {
        for (int i = 0; i < FIELD_SIZE; ++i) {
            for (int j = 0; j < FIELD_SIZE; ++j) {
                grid[i][j] = CELL_EMPTY;
            }
        }
    }

private:
    bool isShipDestroyed(int row, int col) const {
        int c = col;
        while (c >= 0 && grid[row][c] == CELL_HIT) --c;
        if (c >= 0 && grid[row][c] == CELL_SHIP) return false;

        c = col + 1;
        while (c < FIELD_SIZE && grid[row][c] == CELL_HIT) ++c;
        if (c < FIELD_SIZE && grid[row][c] == CELL_SHIP) return false;

        int r = row;
        while (r >= 0 && grid[r][col] == CELL_HIT) --r;
        if (r >= 0 && grid[r][col] == CELL_SHIP) return false;

        r = row + 1;
        while (r < FIELD_SIZE && grid[r][col] == CELL_HIT) ++r;
        if (r < FIELD_SIZE && grid[r][col] == CELL_SHIP) return false;

        return (grid[row][col] == CELL_HIT);
    }

    void MissNeighbors(int row, int col) {
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                int r = row + dr;
                int c = col + dc;
                if (isValidCoord(r, c) && grid[r][c] == CELL_EMPTY) {
                    grid[r][c] = CELL_MISS;
                }
            }
        }
    }
};

class Player {
protected:
    GameField ownField;    
    GameField enemyField;    
    char name[50];           
    int shipSizes[SHIP_TYPES];  
    int shipCounts[SHIP_TYPES];  
    int totalShips;     

public:
    Player(const char* playerName): totalShips(0){
        std::strcpy(name, playerName);

        for (int i = 0; i < SHIP_TYPES; ++i) {
            shipSizes[i] = SHIP_SIZES[i];
            shipCounts[i] = SHIP_COUNTS[i];
            totalShips += SHIP_COUNTS[i];
        }
    }

    Player(const Player& other): ownField(other.ownField), enemyField(other.enemyField), totalShips(other.totalShips){
        std::strcpy(name, other.name);
        for (int i = 0; i < SHIP_TYPES; ++i) {
            shipSizes[i] = other.shipSizes[i];
            shipCounts[i] = other.shipCounts[i];
        }
    }

    Player& operator=(const Player& other) {
        if (this == &other) return *this;

        ownField = other.ownField;
        enemyField = other.enemyField;
        std::strcpy(name, other.name);
        totalShips = other.totalShips;

        for (int i = 0; i < SHIP_TYPES; ++i) {
            shipSizes[i] = other.shipSizes[i];
            shipCounts[i] = other.shipCounts[i];
        }

        return *this;
    }


    virtual ~Player() {}

    virtual void placeShips() = 0;

    virtual Coord makeMove(const GameField& enemyOwnField) = 0;


    GameField& getOwnField() { return ownField; }
    const GameField& getOwnField() const { return ownField; }

    GameField& getEnemyField() { return enemyField; }
    const GameField& getEnemyField() const { return enemyField; }

    bool hasLost() const {
        return ownField.countAliveShips() == 0;
    }

    bool canShoot(int row, int col) const {
        return !enemyField.isAlreadyShot(row, col);
    }

    int receiveShot(int row, int col) {
        return ownField.shoot(row, col);
    }

    void recordShotResult(int row, int col, int result) {
        if (result == 0) {
            enemyField.setCell(row, col, CELL_MISS);
        }
        else if (result == 1) {
            enemyField.setCell(row, col, CELL_HIT);
        }
        else if (result == 2) {
            enemyField.setCell(row, col, CELL_HIT);
            enemyField.markShipAndCell(row, col);
        }
    }

    void autoPlaceShips() {
        ownField.clear();
        std::srand(static_cast<unsigned int>(std::time(nullptr)));


        for (int type = SHIP_TYPES - 1; type >= 0; --type) {
            int size = shipSizes[type];
            int count = shipCounts[type];

            for (int n = 0; n < count; ++n) {
                bool placed = false;
                int attempts = 0;
                const int MAX_ATTEMPTS = 1000;

                while (!placed && attempts < MAX_ATTEMPTS) {
                    int row = std::rand() % FIELD_SIZE;
                    int col = std::rand() % FIELD_SIZE;
                    bool isHorizontal = (std::rand() % 2 == 0);

                    if (ownField.canPlaceShip(row, col, size, isHorizontal)) {
                        ownField.placeShip(row, col, size, isHorizontal);
                        placed = true;
                    }
                    ++attempts;
                }

                if (!placed) {
                    ownField.clear();
                    type = SHIP_TYPES;
                    break;
                }
            }
        }
    }
};

class HumanPlayer : public Player {
public:
    HumanPlayer(const char* playerName) : Player(playerName) {}

    HumanPlayer(const HumanPlayer& other) : Player(other) {}

    HumanPlayer& operator=(const HumanPlayer& other) {
        if (this == &other) return *this;
        Player::operator=(other);
        return *this;
    }

    virtual ~HumanPlayer() {}

    void placeShips() {
        std::cout << "\n=== Ship placement ===\n";

        int choice = 0;
        bool validChoice = false;

        while (!validChoice) {
            std::cout << "1. Auto placement\n";
            std::cout << "2. Manual placement\n";
            std::cout << "Choice: ";
            std::cin >> choice;
            std::cin.ignore(10000, '\n');

            if (choice == 1 || choice == 2) {
                validChoice = true;
            }
            else {
                std::cout << "Error: plese enter only 1 or 2\n\n";
            }
        }

        if (choice == 1) {
            autoPlaceShips();
            std::cout << "Ships were placed\n\n";
        }
        else {
            manualPlaceShips();
        }
    }

    Coord makeMove(const GameField& enemyOwnField) {
        Coord coord;
        char input[10];
        bool valid = false;

        do {
            std::cout << name << ", input coards of shoot (example, A1): ";
            std::cin.getline(input, 10);

            if (std::strlen(input) < 2 || std::strlen(input) > 3) {
                std::cout << "Error: incorrect format\n\n";
                continue;
            }

            char colChar = input[0];
            if (colChar >= 'a' && colChar <= 'j') colChar = colChar - 'a' + 'A';
            if (colChar < 'A' || colChar > 'J') {
                std::cout << "Error: letter is out of range\n\n";
                continue;
            }
            coord.col = colChar - 'A';

            coord.row = std::atoi(input + 1);
            if (coord.row < 0 || coord.row > 9) {
                std::cout << "Error: digit is out of range\n\n";
                continue;
            }

            if (!canShoot(coord.row, coord.col)) {
                std::cout << "You've already shot at this cell, choose another\n\n";
                continue;
            }

            valid = true;
        } while (!valid);

        return coord;
    }

private:
    void manualPlaceShips() {
        ownField.clear();

        std::cout << "\n======= Manual placement =======\n";
        std::cout << "Format: letter (A-J), digit (0-9), direction (H/V).\n";
        std::cout << "Example: A0 H.\n\n";

        for (int type = SHIP_TYPES - 1; type >= 0; --type) {
            int size = shipSizes[type];
            int count = shipCounts[type];

            const char* sizeNames[] = { "single-deck", "double-deck",
                                       "trple-deck", "four-deck" };

            for (int n = 0; n < count; ++n) {
                std::cout << "Place " << sizeNames[type] << " ship (" << size << " deck" << " [" << (n + 1) << "/" << count << "]:\n";

                printOwnField();

                bool placed = false;
                while (!placed) {
                    char input[20];
                    std::cout << "Input coard and direction: ";
                    std::cin.getline(input, 20);

                    if (std::strlen(input) < 4) {
                        std::cout << "Error: incorrect format\n\n";
                        continue;
                    }

                    char colChar = input[0];
                    if (colChar >= 'a' && colChar <= 'j') colChar = colChar - 'a' + 'A';
                    int col = colChar - 'A';

                    char* rest = input + 1;
                    int row = std::atoi(rest);

                    char dirChar = 0;
                    for (int i = 1; input[i] != '\0'; ++i) {
                        if (input[i] == 'H' || input[i] == 'h' ||
                            input[i] == 'V' || input[i] == 'v') {
                            dirChar = input[i];
                            break;
                        }
                    }

                    if (dirChar == 0) {
                        std::cout << "Please, enter direction\n\n";
                        continue;
                    }

                    bool isHorizontal = (dirChar == 'H' || dirChar == 'h');

                    if (col < 0 || col >= FIELD_SIZE || row < 0 || row >= FIELD_SIZE) {
                        std::cout << "Error: coard is out of range\n\n";
                        continue;
                    }

                    if (ownField.canPlaceShip(row, col, size, isHorizontal)) {
                        ownField.placeShip(row, col, size, isHorizontal);
                        placed = true;
                    }
                    else {
                        std::cout << "Incorrect placement. Check ships around cell and boarders \n\n";
                    }
                }
            }
        }

        std::cout << "\nPlacement is complete\n";
        printOwnField();
    }

    void printOwnField() const {
        std::cout << "\n  Your field:\n";
        std::cout << "  A B C D E F G H I J\n";
        for (int i = 0; i < FIELD_SIZE; ++i) {
            std::cout << i << " ";
            for (int j = 0; j < FIELD_SIZE; ++j) {
                CellState cell = ownField.getCell(i, j);
                if (cell == CELL_SHIP) {
                    std::cout << "# ";
                }
                else if (cell == CELL_EMPTY) {
                    std::cout << ". ";
                }
                else {
                    std::cout << ". ";
                }
            }
            std::cout << std::endl;
        }
    }
};

class ComputerPlayer : public Player {
private:
    Coord lastHit;       
    bool hasLastHit;     
    Coord huntStack[100];  
    int huntStackSize;   
    bool usedCells[FIELD_SIZE][FIELD_SIZE];  

public:
    ComputerPlayer(const char* playerName): Player(playerName), hasLastHit(false), huntStackSize(0){
        lastHit.row = -1;
        lastHit.col = -1;
        for (int i = 0; i < FIELD_SIZE; ++i) {
            for (int j = 0; j < FIELD_SIZE; ++j) {
                usedCells[i][j] = false;
            }
        }
    }

    ComputerPlayer(const ComputerPlayer& other): Player(other), lastHit(other.lastHit), hasLastHit(other.hasLastHit), huntStackSize(other.huntStackSize){
        for (int i = 0; i < huntStackSize; ++i) {
            huntStack[i] = other.huntStack[i];
        }
        for (int i = 0; i < FIELD_SIZE; ++i) {
            for (int j = 0; j < FIELD_SIZE; ++j) {
                usedCells[i][j] = other.usedCells[i][j];
            }
        }
    }

    ComputerPlayer& operator=(const ComputerPlayer& other) {
        if (this == &other) return *this;

        Player::operator=(other);

        lastHit = other.lastHit;
        hasLastHit = other.hasLastHit;
        huntStackSize = other.huntStackSize;

        for (int i = 0; i < huntStackSize; ++i) {
            huntStack[i] = other.huntStack[i];
        }
        for (int i = 0; i < FIELD_SIZE; ++i) {
            for (int j = 0; j < FIELD_SIZE; ++j) {
                usedCells[i][j] = other.usedCells[i][j];
            }
        }

        return *this;
    }

    virtual ~ComputerPlayer() {}

    void placeShips() {
        autoPlaceShips();
        std::cout << "Computer placement is complete\n\n";
    }

    Coord makeMove(const GameField& enemyOwnField) {
        Coord coord;
        bool valid = false;

        while (!valid) {
            if (huntStackSize > 0) {
                --huntStackSize;
                coord = huntStack[huntStackSize];
            }
            else {
                int attempts = 0;
                const int MAX_ATTEMPTS = 200;
                do {
                    coord.row = std::rand() % FIELD_SIZE;
                    coord.col = std::rand() % FIELD_SIZE;
                    ++attempts;
                } while ((usedCells[coord.row][coord.col] ||
                    !canShoot(coord.row, coord.col) ||
                    ((coord.row + coord.col) % 2 != 0)) &&
                    attempts < MAX_ATTEMPTS);

                if (attempts >= MAX_ATTEMPTS) {
                    do {
                        coord.row = std::rand() % FIELD_SIZE;
                        coord.col = std::rand() % FIELD_SIZE;
                    } while (!canShoot(coord.row, coord.col));
                }
            }

            if (!usedCells[coord.row][coord.col] && canShoot(coord.row, coord.col)) {
                usedCells[coord.row][coord.col] = true;
                valid = true;
            }
        }

        std::cout << "Computer shoot at  " << static_cast<char>('A' + coord.col)
            << coord.row << std::endl;
        return coord;
    }

    void processShotResult(int row, int col, int result) {
        recordShotResult(row, col, result);

        if (result == 2) {
            hasLastHit = false;
            huntStackSize = 0;
            lastHit.row = -1;
            lastHit.col = -1;
        }
        else if (result == 1) {
            hasLastHit = true;
            lastHit.row = row;
            lastHit.col = col;

            int dr[] = { -1, 1, 0, 0 };
            int dc[] = { 0, 0, -1, 1 };

            for (int k = 0; k < 4; ++k) {
                int newRow = row + dr[k];
                int newCol = col + dc[k];

                if (newRow >= 0 && newRow < FIELD_SIZE && newCol >= 0 && newCol < FIELD_SIZE && !usedCells[newRow][newCol] && canShoot(newRow, newCol) && huntStackSize < 100) {
                    bool alreadyInStack = false;
                    for (int s = 0; s < huntStackSize; ++s) {
                        if (huntStack[s].row == newRow && huntStack[s].col == newCol) {
                            alreadyInStack = true;
                            break;
                        }
                    }

                    if (!alreadyInStack) {
                        huntStack[huntStackSize].row = newRow;
                        huntStack[huntStackSize].col = newCol;
                        ++huntStackSize;
                    }
                }
            }
        }
    }
};


class Game {
private:
    HumanPlayer human;
    ComputerPlayer computer;
    bool gameOver;

public:
    Game(): human("Player"), computer("Computer"), gameOver(false){
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    }

    Game(const Game& other): human(other.human), computer(other.computer), gameOver(other.gameOver){}

    Game& operator=(const Game& other) {
        if (this == &other) return *this;

        human = other.human;
        computer = other.computer;
        gameOver = other.gameOver;

        return *this;
    }

    ~Game() {}

    void run() {
        std::cout << "========== SEA BATTLE ==========\n\n";

        human.placeShips();
        computer.placeShips();

        std::cout << "\n========== START ==========\n";
        std::cout << "First turn is your!\n\n";

        bool humanTurn = true;

        while (!gameOver) {
            if (humanTurn) {
                humanTurn = !humanMove();  
            }
            else {
                humanTurn = computerMove(); 
            }
        }

        std::cout << "\n========== GAME OVER ==========\n";
    }

private:
    bool humanMove() {
        std::cout << "\n--- Your turn ---\n";

        printEnemyField();

        Coord shot = human.makeMove(computer.getOwnField());

        int result = computer.receiveShot(shot.row, shot.col);

        if (result == -1) {
            std::cout << "You've already shot at this cell, choose another\n\n";
            return false; 
        }

        human.recordShotResult(shot.row, shot.col, result);

        if (result == 0) {
            std::cout << "Miss! Computer's turn\n\n";
            return true; 
        }
        else if (result == 1) {
            std::cout << "Hit! Shoot one more time\n";
            if (computer.hasLost()) {
                std::cout << "\n=== YOU WON! ===\n";
                gameOver = true;
            }
            return false;
        }
        else if (result == 2) {
            std::cout << "SHIP IS DESTROYED! Shoot one more time\n";
            if (computer.hasLost()) {
                std::cout << "\n=== YOU WON! ===\n";
                gameOver = true;
            }
            return false;
        }

        return true;
    }

    bool computerMove() {
        std::cout << "\n--- Computer's turn ---\n";

        Coord shot = computer.makeMove(human.getOwnField());

        int result = human.receiveShot(shot.row, shot.col);

        if (result == -1) {
            return false; 
        }

        computer.processShotResult(shot.row, shot.col, result);

        std::cout << "Result: ";
        if (result == 0) {
            std::cout << "Miss! Your turn\n";
            printBothFields();
            return true;
        }
        else if (result == 1) {
            std::cout << "HIT! Computer shoot one more time\n";
            printBothFields();
            if (human.hasLost()) {
                std::cout << "\n=== YOU LOOSE! ===\n";
                gameOver = true;
            }
            return false;
        }
        else if (result == 2) {
            std::cout << "SHIP IS DESTROYED!\n";
            printBothFields();
            if (human.hasLost()) {
                std::cout << "\n=== YOU LOOSE! ===\n";
                gameOver = true;
            }
            return false;
        }

        return true;
    }

    void printEnemyField() const {
        std::cout << "\nEnemy's field (your shots):\n";
        std::cout << "  A B C D E F G H I J\n";

        const GameField& field = human.getEnemyField();

        for (int i = 0; i < FIELD_SIZE; ++i) {
            std::cout << i << " ";
            for (int j = 0; j < FIELD_SIZE; ++j) {
                CellState cell = field.getCell(i, j);
                switch (cell) {
                case CELL_EMPTY:  std::cout << ". "; break;
                case CELL_SHIP:   std::cout << ". "; break;
                case CELL_HIT:    std::cout << "X "; break;
                case CELL_MISS:   std::cout << "* "; break;
                case CELL_DESTROYED: std::cout << "# "; break;
                default:          std::cout << "? "; break;
                }
            }
            std::cout << std::endl;
        }
        std::cout << "  . - unknown, * - miss, X - hit, # - destroyed\n";
    }

    void printBothFields() const {
        std::cout << "\nYour field:          Enemy's field (your shots):\n";
        std::cout << "  A B C D E F G H I J           A B C D E F G H I J\n";

        const GameField& own = human.getOwnField();
        const GameField& enemy = human.getEnemyField();

        for (int i = 0; i < FIELD_SIZE; ++i) {
            std::cout << i << " ";
            for (int j = 0; j < FIELD_SIZE; ++j) {
                CellState cell = own.getCell(i, j);
                switch (cell) {
                case CELL_EMPTY:
                    std::cout << ". ";
                    break;
                case CELL_SHIP:
                    std::cout << "O ";
                    break;
                case CELL_HIT:
                    std::cout << "X ";
                    break;
                case CELL_MISS:
                    std::cout << "* ";
                    break;
                case CELL_DESTROYED:
                    std::cout << "# ";
                    break;
                default:
                    std::cout << "? ";
                    break;
                }
            }
            std::cout << "        " << i << " ";
            for (int j = 0; j < FIELD_SIZE; ++j) {
                CellState cell = enemy.getCell(i, j);
                switch (cell) {
                case CELL_EMPTY:
                    std::cout << ". ";
                    break;
                case CELL_SHIP:
                    std::cout << ". ";
                    break;
                case CELL_HIT:
                    std::cout << "X ";
                    break;
                case CELL_MISS:
                    std::cout << "* ";
                    break;
                case CELL_DESTROYED:
                    std::cout << "# ";
                    break;
                default:
                    std::cout << "? ";
                    break;
                }
            }
            std::cout << std::endl;
        }
        std::cout << "  O - ship cell, . - unknown, * - miss, X - hit, # - destroyed\n";
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}