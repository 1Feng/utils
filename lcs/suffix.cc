#include "suffix.h"
#include <cstring>
#include <map>
#include <algorithm>

#define maxn 20

char ss[] = "aabaaaab";
//char ss[] = "aaaba$abaa";

template<class T, class L>
struct print
{
  void operator()(const std::pair<T, L>& i)
  {
    std::cout << i.first << " " << i.second << '\t';
  }
};


template<class T>
void rank(std::vector<std::pair<unsigned int, T> >& vec)
{
  std::for_each(vec.begin(), vec.end(), print<unsigned int, T>());
  std::cout << std::endl;
  std::vector<std::pair<unsigned int, T> > temp(vec);
  std::sort(temp.begin(), temp.end(), sort_cmp<unsigned int, T>());
  int no = 0;
  std::map<std::pair<unsigned int,T>, int> mm;
  for(size_t i = 0; i < temp.size(); ++i)
  {
    if(mm.count(temp[i]) == 0) mm.insert(std::make_pair(temp[i], no++));
  }
  for(size_t i = 0; i< vec.size(); ++i)
  {
    vec[i].first = mm[vec[i]];
    vec[i].second = 0;
  }
  std::for_each(vec.begin(), vec.end(), print<unsigned int, T>());
  std::cout << std::endl;
}


template<class T>
void getrank(const std::vector<T>& array,
             std::vector<unsigned int>& rank_array)
{
  std::vector<std::pair<unsigned int, unsigned int> > vec;
  {
    std::vector<std::pair<unsigned int, T> > temp;
    for(size_t i = 0; i < array.size(); ++i)
    {
      temp.push_back(std::make_pair(0, array[i]));
    }
    rank(temp);
    for(size_t i = 0; i < temp.size(); ++i)
    {
      vec.push_back(std::make_pair(temp[i].first, 0));
    }
  }
  for(size_t k = 1; k < vec.size(); k *= 2)
  {
    for(size_t i = 0; i < vec.size(); ++i)
    {
      vec[i].first = vec[i].first;
      if(i + k < vec.size())
      {
        vec[i].second = vec[i + k].first;
      }
      else vec[i].second = 0;
    }
    rank(vec);
  }
  rank_array.clear();
  for(size_t i = 0; i < vec.size(); ++i)
  {
    rank_array.push_back(vec[i].first);
  }
}



int main()
{
  std::cout << ss << std::endl;
  std::vector<unsigned int> vec;
  std::vector<unsigned int> rank;
  int len = strlen(ss);
  for(int i = 0; i < len; ++i)
  {
    vec.push_back(ss[i]);
  }
  getrank(vec, rank);
  std::vector<unsigned int> sa(rank.size());
  for(size_t i = 0; i < rank.size(); ++i)
  {
    sa[rank[i]] = i;
    std::cout << rank[i] << '\t';
  }
  std::cout << std::endl;
  for(size_t i = 0; i < sa.size(); ++i)
  {
    std::cout << sa[i] << '\t';
  }
  std::cout << std::endl;
  return 0;
}
