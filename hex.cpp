#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>

using std::cout, std::cin, std::string, std::vector;

enum class hexState {none, red, blue};
enum class direction {center, north, east, south, west};

class Hex
{
  private:
  hexState stateFlag;
  bool isCorner;
  int id;
  direction direction1;
  direction direction2; //For the corner
  vector<Hex*> neighbor;
  
  public: 
  //Constructor
  Hex(int i)
  {
    id = i;
    stateFlag = hexState::none;
    direction1 = direction::center;
    visited = false;
  }
  //Set flag: corner
  void setCorner(direction a, direction b)
  {
    isCorner = true;
    direction1 = a;
    direction2 = b;
  }
  //Connect the hexes, return 0 for a successful connection, return 1 if already full
  int connectHex(Hex* ptr)
  {
    int connected = 6;
    if(isCorner) connected = 3;
    if(direction1 != direction::center) connected = 4;
    if(neighbor.size() < connected)
    {
      neighbor.push_back(ptr);
      return 0;
    }
    else return 1;
  }
  //Setting Edge
  void setEdge(direction a)
  {
    direction1 = a;
  }
  //Code to display the hex in the CLI
  void displayHex()
  {
    switch(stateFlag)
    {
      case hexState::none:
      if (id < 10) cout << "( " << id << " )";
      if (id >= 10 && id < 100) cout << "( " << id << ")";
      if (id >= 100) cout << "(" << id << ")";
      break;
      case hexState::blue:
      cout << "( \033[1;34mX\033[1;0m )";
      break;
      case hexState::red:
      cout << "( \033[1;31mO\033[1;0m )";
      break;
    }
  }
  //Method to access the corner flag  
  bool cornerFlag()
  {
    return isCorner;
  }
  //Method to access the ID of the hex
  int getID()
  {
    return id;
  }
  //Should probably private this attribute and use a method to access this one.
  bool visited;
  //Method to set the color (or X and O) of the hex, will return true if successfully set, will return false if there is already a color
  bool setColor(hexState color)
  {
    if (stateFlag == hexState::none)
    {
      stateFlag = color;
      return true;
    }
    else return false;
  }
  vector<Hex*> getNeighboring()
  {
    return neighbor;
  }
  bool isEdge()
  {
    return (direction1 != direction::center);
  }
  hexState getColor()
  {
    return stateFlag;
  }
};

class Player
{
  private:
  //List of a captured hex for a player object
  vector<Hex*>capturedHex;
  //Identify which is which, X or O
  hexState teamColor;
  //For use in win condition algorithm, to be implemented
  vector<Hex*>longest;
  public:
  //Player initialization
  hexState selectColor(hexState a)
  {
    teamColor = a;
    return a;
  }
  //TODO: Implement win condition algorithm, will probably implement something akin to dijkstra's algorithm
  bool hasWon(direction start);
  //Constructor
  Player(hexState team)
  {
    teamColor = team;
  }
  //Method to capture the hexes, return true if successfully capture, return false if failure
  bool capture(Hex& selectedHex)
  {
    if(selectedHex.visited) return false;
    else 
    {
      capturedHex.push_back(&selectedHex);
      selectedHex.visited = true;
      selectedHex.setColor(teamColor);
      return true;
    }
  }
  //Pathfinding algorithm
  int deploy(Hex* deployed)
  {
    int deployedCount = 0;
    vector<Hex*> neighbor = deployed ->getNeighboring();
    if(!(deployed -> visited))
    {
      for(auto i = neighbor.begin(); i != neighbor.end(); ++i)
      {
        if((*i)->visited == false && (*i)->getColor() == teamColor)
        {
          longest.push_back(*i);
          ++deployedCount;
        }
      }
      return deployedCount;
    }
    else return 0;
  }
};
//Implementation of the win condition algorithm, WIP 
bool Player::hasWon(direction start)
{
  return 1;
  return 0;
}

vector<Hex> initBoard(const int size)
{
  vector<Hex> listHex;
  for(int a = 0; a < size*size; ++a)
  {
    listHex.push_back(Hex(a));
  }
  for(int a = 0; a < size*size; ++a)
  {
    if (a == 0) 
      listHex[a].setCorner(direction::north,direction::west);
    if (a < (size-1) && a != 0) 
      listHex[a].setEdge(direction::north);
    if (a == (size - 1))
      listHex[a].setCorner(direction::north, direction::east);
    if (a % size == 0 && a != 0)
      listHex[a].setEdge(direction::west);
    if (( a + 1 ) % size && a != 0 && a != size*size - 1)
      listHex[a].setEdge(direction::east);
    if (((a > (size*(size - 1))) && (a < size*size - 1)))
      listHex[a].setEdge(direction::south);
    if (a == (size*size - 1))
      listHex[a].setCorner(direction::south, direction::east);
    if (a == (size*(size - 1)))
      listHex[a].setCorner(direction::south, direction::west);
  }
  return listHex;
}

void boardLink(vector<Hex> &board)
{
  int size = board.size();
  int row = (int)sqrt(board.size());
  for(int i = 0; i < size; ++i)
  {
    //top left corner
    if(i == 0)
    {
      board[i].connectHex(&board[1]);
      board[i].connectHex(&board[row]);
    }
    //Top edge
    if(i > 0 && i < row - 1)
    {
      board[i].connectHex(&board[i-1]);
      board[i].connectHex(&board[i+1]);
      board[i].connectHex(&board[i+row-1]);
      board[i].connectHex(&board[i+row]);
    }
    //top right corner
    if(i == row - 1)
    {
      board[i].connectHex(&board[i-1]);
      board[i].connectHex(&board[i+row-1]);
      board[i].connectHex(&board[i+row]);
    }
    //right edge
    if((i+1) % row == 0 && (i+1) < (size))
    {
      board[i].connectHex(&board[i-1]);
      board[i].connectHex(&board[i-row]);
      board[i].connectHex(&board[i+row-1]);
      board[i].connectHex(&board[i+row]);
    }
    //bottom right corner
    if(i == size - 1)
    {
      board[i].connectHex(&board[i-row]);
      board[i].connectHex(&board[i-1]);
    }
  }
}
void render(vector<Hex> &board)
{
  system("cls");
  int offset = 1;
  for(auto i = board.begin(); i != board.end(); ++i)
  {
    i -> displayHex();
    cout << "   ";
    if((i -> getID() + 1) % (int)sqrt(board.size()) == 0) 
    {
      cout << "\n\n";
      for (int a =  0; a < offset; ++a)
      {
        cout << "   ";
      }
      ++offset;
    }
  }
  cout << std::endl;
}

int main()
{
  int size;
  cout << "DEBUG: Insert board size: ";
  cin >> size;
  cout << std::endl;
  vector<Player> playerTest;
  playerTest.push_back(Player(hexState::blue));
  playerTest.push_back(Player(hexState::red));
  vector<Hex> board = initBoard(size);
  render(board);
  bool turnPlayer = 0;
  while(true)
  {
    int move;
    cout << "Player " << turnPlayer + 1 << "'s turn! " <<"Please specify the move: ";
    cin >> move;
    cout << std::endl;
    while(!playerTest[turnPlayer].capture(board[move]))
    {
      cout << "This is not a valid move, please select another! ";
      cin >> move;
      cout << std::endl;
    }
    render(board);
    turnPlayer = !turnPlayer;
  }
}