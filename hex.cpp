#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <windows.h>
#include <ctime>

using std::cout, std::cin, std::string, std::vector;

enum class hexState {none, red, blue};
enum class direction {center, north, east, south, west, unused = -1};
enum class path {vertical, horizontal};
enum class difficulty {not_set, easy, medium};
//Purely cosmetic, can be removed if built to another system
void Color(int color)
{
  //remove the below line & the #include<windows.h> on line 6 to make it work with other system
 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
//The class for the Hex
class Hex
{
  protected:
  hexState stateFlag;
  bool isCorner;
  int id;
  direction direction1;
  direction direction2; //For the corner
  vector<Hex*> neighbor;
  
  public: 
  bool inList = false;
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
    direction2 = direction::unused;
  }
  int deployed = 0;
  inline bool fullyDeployed()
  {
    int size = this->neighbor.size();
    if(this->deployed < size) return 0;
    else return 1;
  }
  //Code to display the hex in the CLI
  void displayHex()
  {
    switch(stateFlag)
    {
      case hexState::none:
      Color(15);
      if (id < 10) cout << "( " << id << " )";
      if (id >= 10 && id < 100) cout << "( " << id << ")";
      if (id >= 100) cout << "(" << id << ")";
      break;
      case hexState::blue:
      cout << "( ";
      Color(1);
      cout << "X";
      Color(15);
      cout << " )";
      break;
      case hexState::red:
      cout << "( ";
      Color(4);
      cout << "O";
      Color(15);
      cout << " )";
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
  direction getEdge1()
  {
    return direction1;
  }
  direction getEdge2()
  {
    return direction2;
  }
  hexState getColor()
  {
    return stateFlag;
  }
};

class Player
{
  protected:
  //List of a captured hex for a player object
  vector<Hex*>capturedHex;
  //Identify which is which, X or O
  hexState teamColor;
  //For use in win condition algorithm, to be implemented
  vector<Hex*>longest;
  path chosenPath;
  bool winFlag;
  direction begin;
  public:
  short int ends;
  //Player initialization
  hexState selectColor(hexState a)
  {
    teamColor = a;
    return a;
  }
  //TODO: Implement win condition algorithm, will probably implement something akin to dijkstra's algorithm
  bool hasWon();
  //Constructor
  Player(hexState team)
  {
    teamColor = team;
    winFlag = 0;
    begin = direction::unused;
    ends = 0;
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
      int i = capturedEdge(selectedHex);
      direction start = getWinningEdge(selectedHex);
        if(begin != start && begin == direction::unused)
        {
          begin = start;
          deploy(&selectedHex);
        } else if(begin == start)
        {
          deploy(&selectedHex);
        }
      }
      return true;
  }
  //
  //Pathfinding algorithm
  int deploy(Hex* deployed)
  {
    direction start = getWinningEdge(*deployed);
    int deployedCount = 0;
    vector<Hex*> neighbor = deployed ->getNeighboring();
    if(deployed->inList == false && start == this -> begin)
    {
      longest.push_back(deployed);
      deployed->inList = true;
    }
    if(!(deployed->fullyDeployed()))
    {
      for(auto i = neighbor.begin(); i != neighbor.end(); ++i)
      {
        if((*i)->inList == false && (*i)->getColor() == teamColor)
        {
          longest.push_back(*i);
          ++deployedCount;
          ++(deployed->deployed);
          (*i)->inList = true;
        }
      }
      return deployedCount;
    }
    else return deployedCount;
  }
  void setDir(int selected_path)
  {
    chosenPath = (path)selected_path;
  }
  int capturedEdge(Hex &captured)
  {
    switch(chosenPath)
    {
      case path::horizontal:
      if(captured.getEdge2() == direction::unused)
      {
        if(captured.getEdge1() == direction::east || captured.getEdge1() == direction::west)
        {
          return 1;
        }
      } else if (captured.getEdge1() == direction::east || captured.getEdge1() == direction::west) return 1; 
      else if (captured.getEdge2() == direction::east || captured.getEdge2() == direction::west) return 2;
      break;
      case path::vertical:
      if(captured.getEdge2() == direction::unused)
      {
        if(captured.getEdge1() == direction::north || captured.getEdge1() == direction::south)
        {
          return 1;
        }
      } else if (captured.getEdge1() == direction::north || captured.getEdge1() == direction::south) return 1; 
      else if (captured.getEdge2() == direction::north || captured.getEdge2() == direction::south) return 2;
      break;
    }
    return 0;
  }
  direction getWinningEdge(Hex &thisHex)
  {
    int i = capturedEdge(thisHex);
    direction start = direction::center;
    if(i != 0)
    {
      switch(i)
      {
        case 1:
        start = thisHex.getEdge1();
        break;
        case 2:
        start = thisHex.getEdge2();
      }
    }
    return start;
  }

};

//Implementation of the win condition algorithm. WERKS 
bool Player::hasWon()
{
  for(int a = 0; a < longest.size(); ++a)
  {
    if(longest[a]->fullyDeployed() == false) deploy(longest[a]);
    switch(begin)
    {
      case direction::east:
      if((longest[a])->getEdge1() == direction::west || (longest[a])->getEdge2() == direction::west) return 1;
      break;
      case direction::west:
      if((longest[a])->getEdge1() == direction::east || (longest[a])->getEdge2() == direction::east) return 1;
      break;
      case direction::north:
      if((longest[a])->getEdge1() == direction::south || (longest[a])->getEdge2() == direction::south) return 1;
      break;
      case direction::south:
      if((longest[a])->getEdge1() == direction::north || (longest[a])->getEdge2() == direction::north) return 1;
      break;
      default:
      return 0;
    } 
    
  }
  return 0;
}
//WIP, will be used later
class AI : public Player
{
  public:
  bool setDifficulty(difficulty diff)
  {
    if(this->diff == difficulty::not_set) 
    {
      this -> diff = diff;
      return 1;
    }
    else return 0;
  }
  int firstMove(vector<Hex> &board);
  int nextMove(vector<Hex> &board);
  private:
  difficulty diff = difficulty::not_set;
};

int AI::firstMove(vector<Hex> &board)
{
  switch(diff)
  {
    case difficulty::easy:
    srand(time(0));
    while(true)
    {
      int a = rand() % board.size();
      if(capture(board[a]))
      {
        return a;
      }
    }
    break;
  }
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
    if (( a + 1 ) % size == 0 && a != size - 1 && a != size*size - 1)
      listHex[a].setEdge(direction::east); 
    if (a % size == 0 && a != 0 && a != size*(size-1))
      listHex[a].setEdge(direction::west);
    if (a == (size*size - 1))
      listHex[a].setCorner(direction::south, direction::east);
    if (((a > (size*(size - 1))) && (a < size*size - 1)))
      listHex[a].setEdge(direction::south);
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
    else if(i > 0 && i < row - 1)
    {
      board[i].connectHex(&board[i-1]);
      board[i].connectHex(&board[i+1]);
      board[i].connectHex(&board[i+row-1]);
      board[i].connectHex(&board[i+row]);
    }
    //top right corner
    else if(i == row - 1)
    {
      board[i].connectHex(&board[i-1]);
      board[i].connectHex(&board[i+row-1]);
      board[i].connectHex(&board[i+row]);
    }
    //right edge
    else if((i+1) % row == 0 && (i+1) < (size))
    {
      board[i].connectHex(&board[i-1]);
      board[i].connectHex(&board[i-row]);
      board[i].connectHex(&board[i+row-1]);
      board[i].connectHex(&board[i+row]);
    }
    //bottom right corner
    else if(i == size - 1)
    {
      board[i].connectHex(&board[i-row]);
      board[i].connectHex(&board[i-1]);
    }
    //Bottom edge
    else if(i > size - row && i < size - 1)
    {
      board[i].connectHex(&board[i-1]);
      board[i].connectHex(&board[i+1]);
      board[i].connectHex(&board[i-row]);
      board[i].connectHex(&board[i-row+1]);
    }
    //bottom left corner
    else if(i == size - row)
    {
      board[i].connectHex(&board[i-row]);
      board[i].connectHex(&board[i-row+1]);
      board[i].connectHex(&board[i+1]);
    }
    //right edge
    else if(i % row == 0 && i != 0 && i != (size - row))
    {
      board[i].connectHex(&board[i-row]);
      board[i].connectHex(&board[i-row+1]);
      board[i].connectHex(&board[i+1]);
      board[i].connectHex(&board[i+row]);
    }
    else
    {
      board[i].connectHex(&board[i-row]);
      board[i].connectHex(&board[i-row+1]);
      board[i].connectHex(&board[i+1]);
      board[i].connectHex(&board[i-1]);
      board[i].connectHex(&board[i+row]);
      board[i].connectHex(&board[i+row-1]);
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
  boardLink(board);
  render(board);
  int player2dir = 0;
  bool turnPlayer = 1;
  cout << "Player " << turnPlayer + 1 << " please select where you want to play:\n";
  cout << "[1] North - South\n" << "[2] East - West\n";
  cin >> player2dir;
  --player2dir;
  playerTest[turnPlayer].setDir(player2dir);
  playerTest[!turnPlayer].setDir(!player2dir);
  while(!playerTest[turnPlayer].hasWon())
  {
    turnPlayer = !turnPlayer;
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
  }
  cout << "Player " << turnPlayer + 1 << " has won!\n";
}