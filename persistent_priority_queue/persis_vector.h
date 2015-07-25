//This is a sample vector serialization element into file by mmap
//head only
#ifndef _SERIA_VECTOR_H_
#define _SERIA_VECTOR_H_
#include <sys/mman.h>
#include <cstring>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <exception>


template <class T>
class SeriaVector
{
public:
  class expend_exception : public std::exception
  {
  public:
    const char* what() const throw()
    {
      return "SeriaVector expend failure!";
    }
  };

public:
  typedef T value_type;
  typedef value_type* iterator;
  typedef const value_type* const_iterator;
  typedef value_type& reference;
  typedef const value_type& const_reference;

  bool Init(const std::string& file_name);

  void Push_back (const_reference val);

  const_reference operator[](size_t i) const
  {
    return *(Begin() + i);
  }

  void Pop_back()
  {
    set_size(Size()-1);
  }

  iterator Begin()
  {
    return (iterator)(head_ + sizeof(size_t));
  }
  const_iterator Begin() const
  {
    return (const_iterator)(head_ + sizeof(size_t));
  }

  iterator End()
  {
    return Begin() + Size();
  }
  const_iterator End() const
  {
    return Begin() + Size();
  }

  const_reference Front() const
  {
    return *Begin();
  }

  bool Empty() const
  {
    return Size() ? false : true;
  }

  size_t Capacity() const
  {
    return ( length_ - sizeof(size_t) - 1) / sizeof(value_type);
  }

  size_t Size() const
  {
    return *(size_t*)head_;
  }

private:
  //load memory from the exsit file
  bool init_vector(const char* file_name);
  // expend file length
  bool expend(size_t n);
  //check file valid
  bool valid() const;

  void set_size(size_t size)
  {
    *(size_t*)head_ = size;
  }

  void save()
  {
    munmap(head_, length_);
  }

private:
  std::string file_name_;
  char* head_;
  size_t length_;
};

template <class T>
bool SeriaVector<T>::Init(const std::string& file_name)
{
  file_name_ = file_name;
  return init_vector(file_name_.c_str());
}

template <class T>
bool SeriaVector<T>::init_vector(const char* file_name)
{
  struct stat st;
  int fd = open(file_name, O_CREAT | O_RDWR, 00666);
  if(-1 == fd)
  {
    return false;
  }
  fstat(fd, &st);
  length_ = st.st_size;
  //first init
  if(length_ == 0)
  {
    length_ = sizeof(size_t) + 0 * sizeof(T) + 1;
    lseek(fd, length_ - 1, SEEK_SET);
    ssize_t res = write(fd, "\0", 1);
    if(-1 == res)
    {
      return false;
    }
    head_ = (char*)mmap(NULL, length_, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    set_size(0);
  }
  else
  {
    head_ = (char*)mmap(NULL, length_, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(!valid())
    {
      return false;
    }
  }
  close(fd);
  return true;
}

template <class T>
void SeriaVector<T>::Push_back (const T& val)
{
  //TODO: expend mmap length
  if(Size() + 1 >= Capacity())
  {
    if(!expend( (Size() + 1) * 2))
    {
      //after this, uexpected exception will occur
      throw expend_exception();
    }
  }

  *End() = val;
  set_size(Size() + 1);
}

template <class T>
bool SeriaVector<T>::valid() const
{
  if(Size() <= Capacity())
  {
    return true;
  }
  else
  {
    return false;
  }
}

template <class T>
bool SeriaVector<T>::expend(size_t n)
{
  save();
  int fd = open(file_name_.c_str(), O_CREAT | O_RDWR, 00666);
  if(-1 == fd)
  {
    return false;
  }
  length_ = sizeof(size_t) + n * sizeof(T) + 1;
  lseek(fd, length_ - 1, SEEK_SET);
  ssize_t res = write(fd, "\0", 1);
  if(-1 == res)
  {
    return false;
  }
  head_ = (char*)mmap(NULL, length_, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if(!valid())
  {
    return false;
  }
  close(fd);
  return true;
}

#endif
