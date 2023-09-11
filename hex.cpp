#include <iostream>
#include <string>
#include <vector>
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
  
  public: //Constructor
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

  void displayHex()
  {
    switch(stateFlag)
    {
      case hexState::none:
      cout << id;
      break;
      case hexState::blue:
      cout << "X";
      break;
      case hexState::red:
      cout << "O";
      break;
    }
  }
  bool cornerFlag()
  {
    return isCorner;
  }
  int getID()
  {
    return id;
  }
  bool visited;
};

class Player
{
  vector<Hex*>capturedHex;
  hexState teamColor;
  vector<Hex*>longest;
  hexState selectColor(hexState a)
  {
    teamColor = a;
    return a;
  }
  bool hasWon(direction start);
};

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
    if (a == (size*size - 1)) listHex[a].setCorner(direction::south, direction::east);
    if (a == (size*(size - 1))) listHex[a].setCorner(direction::south, direction::west);
  }
  return listHex;
}

int main()
{
  vector<Hex> board = initBoard(5);
  for(auto i = board.begin(); i != board.end(); ++i)
  {
    i -> displayHex();
    cout << "\t";
    if((i -> getID() + 1) % 5 == 0) cout << "\n";
  }
}