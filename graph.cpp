#include <iostream>
#include <cstdlib>
#include <vector>
#include <iterator>
#include <fstream>
#include <string>
#include <sstream>
//Updated from my Dijkstra's algorithm program, added file reading functionality and prim's MST algorithm.
//By Quan (2023)
const int INFINITY = -1;
struct node
{
  int index;  //The name of the node
  int distance;
  //Visited flag
  bool is_visited;
  //Constructor, the first one is for the start node
  node()
  {
    this->index = 0;
    this->distance = 0;
  }
  node(int inx, int dist)
  {
    this->index = inx;
    this->distance = dist;
    this->is_visited = false;
  }
};

//Initiate a new list with -1 being infinity
std::vector<node> create_list(int count)
{
  std::vector<node> list;
  for(int i = 0; i < count; ++i)
  {
    list.push_back(node(i,INFINITY));  //-1 denotes an unknown node
  }
  list[0].distance = 0;
  return list;
}
//Check the list created above, return the index of the closest unvisited node that is not the root
int get_min(std::vector<node> &list)
{
  int min_val;
  int min_index = -1;
  //Get first value to check for min
  for (int a = 0; a < list.size(); ++a)
  {
    if(list[a].distance != 0 && list[a].distance != INFINITY && list[a].is_visited == false)
    {
      min_val = list[a].distance;
      min_index = list[a].index;
      break;
    }
  }
  
  for(int i = 1; i < list.size(); ++i)
  {
    if((list[i].is_visited == false && list[i].distance > 0) && (list[i].distance < min_val))
    {
        min_val = list[i].distance;
        min_index = i;
    }
  }
  return min_index;
}

//Discover new nodes, return the number of new nodes discovered
int deploy_dijkstra(node &target, std::vector<node> &list, int** connectivity_matrix)
{
  int dist = target.distance;
  int index = target.index;
  int deployed = 0;
  for(int a = 0; a < list.size(); ++a)
  {
    if(list[a].is_visited == false && connectivity_matrix[index][a] != 0 && (list[a].distance == INFINITY || list[a].distance > dist+connectivity_matrix[index][a]))
    {
      list[a].distance = dist+connectivity_matrix[index][a];
      ++deployed;
    }
  }
  target.is_visited = true;
  return deployed;
}

//a derivation of dijkstra
int deploy_prim(node &target, std::vector<node> &list, int** connectivity_matrix)
{
  int dist = target.distance;
  int index = target.index;
  int deployed = 0;
  //The only part that's different is that this algorithm does not accumulate the distance in the beginning node
  for(int a = 0; a < list.size(); ++a)
  {
    if(list[a].is_visited == false && connectivity_matrix[index][a] != 0 && (list[a].distance == INFINITY || list[a].distance > connectivity_matrix[index][a]))
    {
      list[a].distance = connectivity_matrix[index][a];
      ++deployed;
    }
  }
  target.is_visited = true;
  return deployed;
}

//Create shortest path tree
void dijkstra(std::vector<node> &list, int** connectivity)
{
  
  int i = 0;
  int a = 1;
  deploy_dijkstra(list[0], list, connectivity);  //Discover the first nodes
  i = get_min(list);
  //While new nodes are discoverable, loop and get min
  while(deploy_dijkstra(list[i], list, connectivity) > 0)
  {
    i = get_min(list);
  }
}

void prim(std::vector<node> &list, int** connectivity, std::vector<int> &list_name)
{
  //Slightly different so as to print out the names of the nodes
  int i = 0;
  int a = 1;
  list_name.push_back(i);
  deploy_prim(list[0], list, connectivity);  //Discover the first nodes
  i = get_min(list);
  list_name.push_back(i);
  //While new nodes are discoverable, loop and get min
  while(deploy_prim(list[i], list, connectivity) > 0)
  {
    i = get_min(list);
    list_name.push_back(i);
  }
  i = get_min(list);
  if (i != -1 ) list_name.push_back(i);
}



int main()
{
  int choice_1 = 0;
//  srand(time(0));
//This is the choice part, mostly for testing purposes
  std::cout << "Select one option: \n" << "\t[1] Testing matrix\n" << "\t[2] Entering matrix from a file\n";
  std::cin >> choice_1;
  if(choice_1 == 1)
  {
    const int count = 6;
//  int** matrix = new int*[count]();
//  for(int i = 0; i < count;++i)
//  {
//    matrix[i] = new int[count]();
//  }
//  for(int i = 0; i < count; ++i)
//  {
//    for(int j = 0; j < count; ++j)
//    {
//      if(i==j) matrix[i][j] = 0;
//      else if (bool prob = (rand() % 100) < 20)
//      {
//        matrix[i][j] = matrix[j][i] = rand() % 100; //For convenience, the average distance would be from 0 to 50
//      }
//    }
//  }

    int row_1[6]={0,1,0,0,4,0};
    int row_2[6]={1,0,2,0,6,0};
    int row_3[6]={0,2,0,3,0,8};
    int row_4[6]={0,0,3,0,0,2};
    int row_5[6]={4,6,0,0,0,4};
    int row_6[6]={0,0,8,2,4,0};
    int* matrix[6] = {row_1, row_2, row_3, row_4, row_5, row_6};
    int** ptr_to_matrix = matrix;
    std::vector<node> final = create_list(count);
    std::cout << "Press [1] for Dijkstra, press [2] for prim\n";
    int choice = 0;
    std::cin >> choice;
    if(choice == 1)
    {
      dijkstra(final, ptr_to_matrix);
      for(int i = 0; i < final.size(); ++i) std::cout << final[i].distance << "\t";
      double avg = 0;
      auto a = final.begin();
      int c = count;
      for (auto i = final.begin(); i != final.end(); ++i)
      {
        if (i->distance == 0) final.erase(i);
      }
      for(int i = 1; i <= final.size();++i)
      {
        avg=(avg*(i-1) + (a->distance))/i;
        ++a;
      }
      std::cout << std::endl << "the average is shortest path: " << avg;
    }
    if(choice == 2)
    {
      std::vector<int> printed_tree;
      prim(final, ptr_to_matrix, printed_tree);
      for(int i = 0; i < printed_tree.size(); ++i) std::cout << printed_tree[i] << "\t";
      int sum = 0;
      for(int i = 0; i < printed_tree.size(); ++i)
      {
        sum += final[printed_tree[i]].distance;
      }
      std::cout << std::endl << "the total weight of the tree is: " << sum;
    }

  }
  //File input, the format for the file should be as followed
  // x x x x x
  // x x x x x
  // x x x x x
  // x x x x x
  // x x x x x
  //Where x is the distance between nodes
  if(choice_1 == 2)
  {
    std::cout << "Enter the name of the word file:\n";
    std::string file_name;
    std::cin >> file_name;
    std::cout << "How many nodes does your file have?\n";
    int count = 0;
    std::cin >> count;
    std::ifstream file(file_name);
    int** ptr_to_matrix = new int*[count];
    for(int a = 0; a < count; ++a)
    {
      std::string row;
      std::getline(file, row);
      int * row_int = new int[count];
      std::istringstream line_stream(row);
      for(int b = 0; b < count; ++b)
      {
        line_stream >> row_int[b];
      }
      ptr_to_matrix[a] = &row_int[0];
    }
    //The same implementation as above
    std::vector<node> final = create_list(count);
    std::cout << "Press [1] for Dijkstra, press [2] for prim\n";
    int choice = 0;
    std::cin >> choice;
    if(choice == 1)
    {
      dijkstra(final, ptr_to_matrix);
      for(int i = 0; i < final.size(); ++i) std::cout << final[i].distance << "\t";
      double avg = 0;
      auto a = final.begin();
      int c = count;
      for (auto i = final.begin(); i != final.end(); ++i)
      {
        if (i->distance == 0) final.erase(i);
      }
      for(int i = 1; i <= final.size();++i)
      {
        avg=(avg*(i-1) + (a->distance))/i;
        ++a;
      }
      std::cout << std::endl << "the average is shortest path: " << avg;
    }
    if(choice == 2)
    {
      std::vector<int> printed_tree;
      prim(final, ptr_to_matrix, printed_tree);
      for(int i = 0; i < printed_tree.size(); ++i) std::cout << printed_tree[i] << "\t";
      int sum = 0;
      for(int i = 0; i < printed_tree.size(); ++i)
      {
        sum += final[printed_tree[i]].distance;
      }
      std::cout << std::endl << "the total weight of the tree is: " << sum;
    }
    //I should not be copying the same block of codes in this part, but oh well
  }
}