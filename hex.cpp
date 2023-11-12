#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <windows.h>
#include <ctime>
#include <random>
#include <algorithm>


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
  int deployed = 0;
  bool visited;
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
      neighbor.push_back(ptr);
      return 0;
  }
  //Setting Edge
  void setEdge(direction a)
  {
    direction1 = a;
    direction2 = direction::unused;
  }
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
  void clearColor()
  {
    stateFlag = hexState::none;
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
  int score = 0;
//
};

void ClearAllHex(vector<Hex> &Board)
{
  for(auto iter = Board.begin(); iter != Board.end(); ++iter)
  {
    iter->visited = 0;
    iter->deployed = 0;
    iter->inList = false;
    iter->clearColor();
  }
}

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
  direction begin;
  public:
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
    begin = direction::center;
  }
  //Method to capture the hexes, return true if successfully capture, return false if failure
  virtual bool capture(Hex& selectedHex)
  {
    if(selectedHex.visited) return false;
    else 
    {
      capturedHex.push_back(&selectedHex);
      selectedHex.visited = true;
      selectedHex.setColor(teamColor);
      int i = getEdgeCornerbyDirection(selectedHex);
      direction start = getWinningEdge(selectedHex);
        if(begin != start && begin == direction::center)
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
  //Based on the chosen path, return which corner to use.
  int getEdgeCornerbyDirection(Hex &captured)
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
  //Get one of the two ends of the winning edge.
  direction getWinningEdge(Hex &thisHex)
  {
    int i = getEdgeCornerbyDirection(thisHex);
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
  void clearAll()
  {
    capturedHex.clear();
    begin = direction::center;
    longest.clear();
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
class AIdiot : public Player
{
  public:
  vector<int> initBoardAI(int times, Player &otherPlayer, vector<Hex> &board)
  {
    vector<int> returnedSheet(board.size(), 0);
    for(int a = 0; a < times; ++a)
    {
      vector<int> scoreSheet;
      for (int a = 0; a < board.size(); ++a)
      {
        scoreSheet.push_back(a);
      }
      std::random_device rd;
      std::default_random_engine random(rd());
      std::shuffle(scoreSheet.begin(), scoreSheet.end(), random);
      for (int a = 0; a < board.size(); ++a)
      {
        if(a & 1) capture(board[scoreSheet[a]]);
        else otherPlayer.capture(board[scoreSheet[a]]);
      }
      if(this->hasWon())
      {
        for(int a = 0; a < board.size(); ++a)
        {
          if(board[a].getColor() == this->teamColor) ++returnedSheet[a];
        }
      }
      ClearAllHex(board);
      this->clearAll();
      otherPlayer.clearAll();
    }
    return returnedSheet;
  }
  AIdiot(hexState team) : Player(team) {}
};


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
  //system("cls");

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

  vector<Hex> board = initBoard(size);
  boardLink(board);
  int isVSAI = false;
  cout << "2-Player or 1-Player mode: \n";
  cout << "[1] One-Player Mode\n" << "[2] Two-player Mode\n";
  cin >> isVSAI;
  if(!(isVSAI - 1))
  {
    AIdiot AIMain = AIdiot(hexState::red);
    AIdiot AISimlulateHuman = AIdiot(hexState::blue);
    cout << "Please select where you want to play:\n";
    cout << "[1] East - West \n" << "[2] North - South\n";
    int player2dir = 0;
    cin >> player2dir;
    --player2dir;
    AIMain.setDir(player2dir);
    AISimlulateHuman.setDir(!player2dir);
    vector<int> scoreBoard = AIMain.initBoardAI(500000, AISimlulateHuman, board);
    AISimlulateHuman.~AIdiot();
    render(board);
    Player Human = Player(hexState::blue);
    Human.setDir(!player2dir);
    Player *winPointer = &AIMain;
    while(!winPointer->hasWon())
    {
      if(winPointer == &AIMain) 
      {
        winPointer = &Human; 
        int move;
        cout << "Please enter your move:";
        cin >> move;
        cout << std::endl;
        while(!winPointer -> capture(board[move]))
        {
          cout << "This is not a valid move, please select another! ";
          cin >> move;
          cout << std::endl;
        }
        render(board);
      }
      else
      {
        winPointer = &AIMain;
        int move;
        auto finded = std::max_element(scoreBoard.begin(), scoreBoard.end());
        move = finded-scoreBoard.begin();
        while(!winPointer -> capture(board[move])) 
        {
          *finded = -1;
          finded = std::max_element(scoreBoard.begin(), scoreBoard.end());
          move = finded-scoreBoard.begin();
        }
        render(board);
      }
    }
    if(winPointer != &AIMain) cout << "You" << " have won!\n";
    else cout << "You" << " have lost!\n";
  }
  else
  {
    vector<Player> playerTest;
    playerTest.push_back(Player(hexState::blue));
    playerTest.push_back(Player(hexState::red));
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
}