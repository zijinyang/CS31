// bees.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the trivial
// functions (marked TRIVIAL).  Then get Room::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;  // max number of rows in the room
const int MAXCOLS = 25;  // max number of columns in the room
const int MAXBEES = 150; // max number of bees allowed
const int INITIAL_BEE_HEALTH = 2;

const int UP = 0;
const int DOWN = 1;
const int LEFT = 2;
const int RIGHT = 3;
const int NUMDIRS = 4;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Room; // This is needed to let the compiler know that Room is a
            // type name, since it's mentioned in the Bee declaration.

class Bee
{
public:
    // Constructor
    Bee(Room *rp, int r, int c);

    // Accessors
    int row() const;
    int col() const;
    int health() const;

    // Mutators
    void move();
    bool getSwatted(int dir);

private:
    Room *m_room;
    int m_row;
    int m_col;
    int m_health;
};

class Player
{
public:
    // Constructor
    Player(Room *rp, int r, int c);

    // Accessors
    int row() const;
    int col() const;
    int age() const;
    bool isDead() const;

    // Mutators
    void stand();
    void moveOrSwat(int dir);
    void setDead();

private:
    Room *m_room;
    int m_row;
    int m_col;
    int m_age;
    bool m_dead;
};

class Room
{
public:
    // Constructor/destructor
    Room(int nRows, int nCols);
    ~Room();

    // Accessors
    int rows() const;
    int cols() const;
    Player *player() const;
    int beeCount() const;
    int numBeesAt(int r, int c) const;
    bool determineNewPosition(int &r, int &c, int dir) const;
    void display() const;

    // Mutators
    bool addBee(int r, int c);
    bool addPlayer(int r, int c);
    bool swatBeeAt(int r, int c, int dir);
    bool moveBees();

private:
    int m_rows;
    int m_cols;
    Player *m_player;
    Bee *m_bees[MAXBEES];
    int m_nBees;
};

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nBees);
    ~Game();

    // Mutators
    void play();

private:
    Room *m_room;
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir);
int randInt(int min, int max);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Bee implementation
///////////////////////////////////////////////////////////////////////////

Bee::Bee(Room *rp, int r, int c)
{
    if (rp == nullptr)
    {
        cout << "***** A bee must be created in some Room!" << endl;
        exit(1);
    }
    if (r < 1 || r > rp->rows() || c < 1 || c > rp->cols())
    {
        cout << "***** Bee created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    m_room = rp;
    m_row = r;
    m_col = c;
    m_health = 2;
}

int Bee::row() const
{
    return m_row;
}

int Bee::col() const
{
    return m_col;
}

int Bee::health() const
{
    return m_health;
}

void Bee::move()
{
    // Attempt to move in a random direction; if bee can't move, don't move
    int dir = randInt(0, NUMDIRS - 1); // dir is now UP, DOWN, LEFT, or RIGHT

    m_room->determineNewPosition(m_row, m_col, dir);
}

bool Bee::getSwatted(int dir) // return true if dies
{
    if(m_health == 1){
        return true;
    }
    m_health--;
    if(!m_room->determineNewPosition(m_row, m_col, dir)){
        return true;
    }
    return false; 
}

///////////////////////////////////////////////////////////////////////////
//  Player implementations
///////////////////////////////////////////////////////////////////////////

Player::Player(Room *rp, int r, int c)
{
    if (rp == nullptr)
    {
        cout << "***** The player must be created in some Room!" << endl;
        exit(1);
    }
    if (r < 1 || r > rp->rows() || c < 1 || c > rp->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_room = rp;
    m_row = r;
    m_col = c;
    m_age = 0;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

int Player::age() const
{
    return m_age;
}

void Player::stand()
{
    m_age++;
}

// If there is a bee adjacent to the player in the direction
// dir, swat it.  Otherwise, move the player to that position if
// possible (i.e., if the move would not be out of the room).
void Player::moveOrSwat(int dir)
{
    m_age++;
    int tempCol = m_col;
    int tempRow = m_row;
    m_room->determineNewPosition(tempRow, tempCol, dir);
    int numBees = m_room->numBeesAt(tempRow, tempCol);
    if(numBees > 0){
        m_room->swatBeeAt(tempRow, tempCol, dir); 
    }else{
        m_room->determineNewPosition(m_row, m_col, dir);
    } 
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Room implementations
///////////////////////////////////////////////////////////////////////////

Room::Room(int nRows, int nCols)
{
    if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
    {
        cout << "***** Room created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nBees = 0;
}

// Delete the player and all remaining dynamically allocated bees.
Room::~Room()
{
    delete m_player;
    for(int i{0}; i < m_nBees;i++){
        if(m_bees[i] != nullptr){
            delete m_bees[i];
        }
    }
}

int Room::rows() const
{
    return m_rows;
}

int Room::cols() const
{
    return m_cols;
}

Player *Room::player() const
{
    return m_player;
}

int Room::beeCount() const
{
    return m_nBees;
}

//Return the number of bees at row r, column c.
int Room::numBeesAt(int r, int c) const
{
    int numBees = 0;
    for(int i{0}; i < beeCount(); i++){
        if(m_bees[i]-> row() == r && m_bees[i] -> col() == c){
            numBees++;
        }
    }
    return numBees; 
}

// If a move from row r, column c, one step in direction dir
// would go out of the room, leave r and c unchanged and return false.
// Otherwise, set r or c so that row r, column c, is now the new
// position resulting from the proposed move, and return true.
bool Room::determineNewPosition(int &r, int &c, int dir) const
{
    switch (dir)
    {
    case UP:
        if(r <= 1){
            return false;
            break;
        }
        r--;
        break;
    case DOWN:
        if(r >= m_rows){
            return false;
            break;
        }
        r++;
        break;
    case LEFT:
        if(c <= 1){
            return false;
            break;
        }
        c--;
        break;
    case RIGHT:
        if(c >= m_cols){
            return false;
            break;
        }
        c++;
        break;
    default:
        return false;
    }
    return true;
}

void Room::display() const
{
    // Position (row,col) in the room coordinate system is represented in
    // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;

    // Fill the grid with dots
    for (r = 0; r < rows(); r++)
        for (c = 0; c < cols(); c++)
            grid[r][c] = '.';

    // Indicate each bee's position
    // If one bee is at some grid point, set the char to 'B'.
    // If it's 2 though 8, set it to '2' through '8'.
    // For 9 or more, set it to '9'.
    for (int i = 0; i < m_nBees; i++)
    {
        Bee *bee = m_bees[i];
        if(bee == nullptr)
            continue;
        char &spot = grid[bee->row() - 1][bee->col() - 1];
        if (spot == '.')
        {
            spot = 'B';
        }
        else if (spot == 'B')
        {
            spot = '2';
        }
        else if (spot == '9')
        {
            spot = '9';
        }
        else
        {
            spot++;
        }
    }

    // Indicate player's position
    if (m_player != nullptr)
    {
        // Set the char to '@', unless there's also a bee there,
        // in which case set it to '*'.
        char &gridChar = grid[m_player->row() - 1][m_player->col() - 1];
        if (gridChar == '.')
            gridChar = '@';
        else
            gridChar = '*';
    }

    // Draw the grid
    clearScreen();
    for (r = 0; r < rows(); r++)
    {
        for (c = 0; c < cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;

    // Write message, bee, and player info
    cout << endl;
    cout << "There are " << beeCount() << " bees remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player." << endl;
    else
    {
        if (m_player->age() > 0)
            cout << "The player has lasted " << m_player->age() << " steps." << endl;
        if (m_player->isDead())
            cout << "The player is dead." << endl;
    }
}

// If there are MAXBEES bees, return false.  Otherwise,
// dynamically allocate a new Bee at coordinates (r,c).  Save the
// pointer to the newly allocated Bee and return true.

// Your function must work as specified in the preceding paragraph even
// in this scenario (which won't occur in this game):  MAXBEES
// are added, then some are destroyed, then more are added.
bool Room::addBee(int r, int c)
{
    if (m_nBees == MAXBEES)
    {
        return false;
    }
    m_bees[m_nBees] = new Bee(this, r, c);
    m_nBees++;
    return true;
}

bool Room::addPlayer(int r, int c)
{
    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

    // Dynamically allocate a new Player and add it to the room
    m_player = new Player(this, r, c);
    return true;
}

// Swat one bee at row r, column c if at least one is at
// that position.  If the bee does not survive the swat, destroy the
// bee object, removing it from the room, and return true.  Otherwise,
// return false (no bee at (r,c), or bee didn't die).
bool Room::swatBeeAt(int r, int c, int dir)
{
    cout << r << ", " << c << endl;
    if(numBeesAt(r,c) <= 0){
        return false;
    }
    for(int i{0}; i < m_nBees; i++){
        Bee*& bee = m_bees[i];
        if(bee == nullptr){
            continue;
        }
        if(bee->row() == r && bee->col() == c){
            bool died = bee->getSwatted(dir);
            if(died){
                delete bee;
                bee = nullptr;
                for(int j{i}; j < m_nBees; j++){
                    m_bees[j] = m_bees[j+1];
                }
                m_bees[m_nBees - 1] = nullptr;
                m_nBees--;
                return true;
            }
            return false; 
        }
    }
    
    return false;
}

bool Room::moveBees()
{
    for (int k = 0; k < m_nBees; k++)
    {
        //Have the k-th bee in the room make one move.
        //If that move results in that bee being in the same
        //position as the player, the player dies.
        Bee *bee = m_bees[k];
        if(bee == nullptr)
            continue;
        bee->move();
        if(bee->row() == m_player->row() && bee->col() == m_player->col()){
            m_player->setDead();
        }
    }

    return !m_player->isDead();
}

///////////////////////////////////////////////////////////////////////////
//  Game implementations
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nBees)
{
    if (nBees < 0)
    {
        cout << "***** Cannot create Game with negative number of bees!" << endl;
        exit(1);
    }
    if (nBees > MAXBEES)
    {
        cout << "***** Trying to create Game with " << nBees
             << " bees; only " << MAXBEES << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1 && cols == 1 && nBees > 0)
    {
        cout << "***** Cannot create Game with nowhere to place the bees!" << endl;
        exit(1);
    }

    // Create room
    m_room = new Room(rows, cols);

    // Add player
    int rPlayer = randInt(1, rows);
    int cPlayer = randInt(1, cols);
    m_room->addPlayer(rPlayer, cPlayer);

    // Populate with bees
    while (nBees > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        // Don't put a bee where the player is
        if (r == rPlayer && c == cPlayer)
            continue;
        // cout << r << ',' << c << endl;
        m_room->addBee(r, c);
        nBees--;
    }
}

Game::~Game()
{
    delete m_room;
}

void Game::play()
{
    m_room->display();
    Player *player = m_room->player();
    if (player == nullptr)
        return;
    while (!player->isDead() && m_room->beeCount() > 0)
    {
        cout << endl;
        cout << "Move (u/d/l/r/q or nothing): ";
        string action;
        getline(cin, action);
        if (action.size() == 0) // player stands
            player->stand();
        else
        {
            switch (action[0])
            {
            default:                  // if bad move, nobody moves
                cout << '\a' << endl; // beep
                continue;
            case 'q':
                return;
            case 'u':
            case 'd':
            case 'l':
            case 'r':
                player->moveOrSwat(decodeDirection(action[0]));
                break;
            }
        }
        m_room->moveBees();
        m_room->display();
    }
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir)
{
    switch (dir)
    {
    case 'u':
        return UP;
    case 'd':
        return DOWN;
    case 'l':
        return LEFT;
    case 'r':
        return RIGHT;
    }
    return -1; // bad argument passed in!
}

// Return a uniformly distributed random int from min to max, inclusive
int randInt(int min, int max)
{
    if (max < min)
        swap(max, min);
    static random_device rnd;
    static default_random_engine generator(rnd());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////

int main()
{
    // Create a game
    // Use this instead to create a mini-game:   Game g(3, 4, 2);
    Game g(7, 8, 25);
    // Game g(3, 4, 2);

    // Play the game
    g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANYTHING BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _WIN32

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = {0, 0};
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                               &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else // not _WIN32

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen() // will just write a newline in an Xcode output window
{
    static const char *term = getenv("TERM");
    if (term == nullptr || strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char *ESC_SEQ = "\x1B["; // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif

