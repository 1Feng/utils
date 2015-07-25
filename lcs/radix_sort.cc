#include "suffix.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

int main()
{
  srand (time(NULL));
  std::vector<int> v;
  for(int i = 0; i < 50; ++i)
  {
    int a = rand() % 200;
    v.push_back(a);
  }
  RadixVector vec;
  for(size_t i = 0; i < v.size(); ++i)
  {
    std::cout << v[i] << std::endl;
    std::vector<unsigned int> temp;
    int y = v[i] % 10;
    temp.push_back(y);
    int x = v[i] / 10;
    if(x != 0) temp.push_back(x);
    vec.push_back(temp);
    //std::cout << x << '\t' << y << std::endl;
  }
  std::cout << "---------------------" << std::endl;
  RadixSort(vec);
  print_vector(vec);
  //std::cout << "---------------------" << std::endl;
  //std::sort(v.begin(), v.end());
  //for(size_t i = 0; i < v.size(); ++i)
  //{
  //  std::cout << v[i] << std::endl;
  //}
  return 0;
}
