#include <iostream>
#include <vector>

typedef std::vector<std::vector<unsigned int> > RadixVector;
typedef std::vector<std::vector<std::vector<unsigned int> > > BucketVector;

template<class T, class L>
struct sort_cmp
{
  bool operator()(std::pair<T, L> a, std::pair<T, L> b)
  {
    if (a.first < b.first ||
       (a.first == b.first && a.second < b.second)) return true;
    return false;
  }
};

void print_vector(const RadixVector& array)
{
  for(size_t i = 0; i < array.size(); ++i)
  {
    for(int j = array[i].size() - 1; j >= 0; --j)
      std::cout << array[i][j] << "\t";
    std::cout << std::endl;
  }
}

void RadixSort(RadixVector& array, unsigned int bucket_len = 0, size_t max = 0)
{
  if(bucket_len == 0 || max == 0)
  {
    for(size_t i = 0; i < array.size(); ++i)
    {
      max = max < array[i].size() ? array[i].size() : max;
      for(size_t j = 0; j < array[i].size(); ++j)
      {
        bucket_len = bucket_len < array[i][j] ? array[i][j] : bucket_len;
      }
    }
  }
 // std::cout << "max_len " << max << " bucket_len " << bucket_len << std::endl;
  BucketVector buckets(bucket_len + 1);

  for(size_t j = 0; j < max; ++j)
  {
    for(size_t i = 0; i < array.size(); ++i)
    {
      if(j < array[i].size())
      {
        buckets[array[i][j]].push_back(array[i]);
      }
      else
      {
        buckets[0].push_back(array[i]);
      }
    }
    RadixVector().swap(array);
    for(size_t i = 0; i < buckets.size(); ++i)
    {
      for(size_t k = 0; k < buckets[i].size(); ++k)
      {
        array.push_back(buckets[i][k]);
      }
    }
    BucketVector(buckets.size()).swap(buckets);
  }
}

template<class T>
struct cmp
{
  int operator()(std::vector<T> v1, std::vector<T> v2)
  {
    for(size_t i = 0; i < v1.size(); ++i)
    {
      if(i >= v2.size() or v1[i] > v2[i]) return 1;
      if(v1[i] < v2[i]) return -1;
    }
    return 0;
  }
};
