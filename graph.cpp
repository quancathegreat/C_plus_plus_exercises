#include <iostream>
class node
{
  public:
  //Constructor
  node(int a = -1)
  {
    this->index = a;
  }
  //Method for accessing the rows
  bool* is_connected()
  const
  {
    //For testing purposes:
    //for (int a = 0; a < count; ++a)
    //std::cout << (this->connected[a]) << "\t";
    //std::cout << std::endl;
    return this->connected;
  }
  //Mapping the connection onto the matrix row
  void map(const int& count)
  {
    this->total_nodes = count;
    this->connected = new bool[count]();
  }
  
  private:
  //The row of the connectivity matrix, will be connected later to form the full matrix using a pointer-to-pointer
  bool* connected;
  //This would be used later
  int total_nodes;
  //To identify the nodes, will be defaulted to -1
  int index;
};
//Create a connectivity matrix. Utilizing pointer-to-pointer  
bool** create_matrix(node* &node_array, const int &count)
{
  bool** full_matrix = new bool*[count];
  for (int i = 0; i < count; ++i)
  {
    full_matrix[i] = node_array[i].is_connected();
  }
  return full_matrix;
}


int main()
{
  const int count = 5;
  node* test = new node[count];
  for(int i = 0; i < count; ++i) 
  {
    test[i] = node(i);
    test[i].map(count);
  }
  bool** matrix = create_matrix(test, count);
  for(int i = 0; i < count; ++i)
  {
    for(int j = 0; j < count; ++j)
    std::cout << matrix[i][j] << "\t";
    std::cout << std::endl;
  }
}