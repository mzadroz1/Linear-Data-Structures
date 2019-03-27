#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class Vector
{
public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:
  pointer vec;
  int length;
  int capacity;

  void resize()
  {
    capacity*=2;
    pointer newVec=new value_type[capacity];
    for(int i=0;i<length;i++)
    {
        newVec[i]=vec[i];
    }
    delete [] vec;
    vec=newVec;
  }

public:
  Vector()
  {
    vec=new value_type[2];
    length=0;
    capacity=2;
  }

  Vector(std::initializer_list<Type> l):Vector()
  {
    for(auto it=l.begin();it!=l.end();it++)
      append(*it);
  }

  Vector(const Vector& other):Vector()
  {
    for(auto it=other.begin();it!=other.end();it++)
      append(*it);
  }

  Vector(Vector&& other)
  {
    vec=other.vec;
    length=other.length;
    capacity=other.capacity;
    other.vec=nullptr;
    other.length=0;
    other.capacity=2;
  }

  ~Vector()
  {
    delete [] vec;
  }

  bool operator==(const Vector& other) const
  {
    if(vec==other.vec && length==other.length && capacity==other.capacity)
        return true;
    return false;
  }

  Vector& operator=(const Vector& other)
  {
    if(*this==other) return *this;

    while(!isEmpty())
      erase(begin());
    for(auto it=other.begin();it!=other.end();it++)
      append(*it);

    return *this;
  }

  Vector& operator=(Vector&& other)
  {
    pointer temp=vec;

    vec=other.vec;
    length=other.length;
    capacity=other.capacity;
    other.vec=nullptr;
    other.length=0;
    other.capacity=2;

    delete [] temp;
    return *this;
  }

  bool isEmpty() const
  {
    return length==0;
  }

  size_type getSize() const
  {
    return length;
  }

  void append(const Type& item)
  {
    if(length+1==capacity)
        resize();
    vec[length++]=item;
  }

  void prepend(const Type& item)
  {
    if(length+1==capacity)
        resize();
    for(int i=length;i>0;i--)
        vec[i]=vec[i-1];
    vec[0]=item;
    length++;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    if(length+1==capacity)
        resize();
    for(int i=length;i>insertPosition.index;i--)
        vec[i]=vec[i-1];
    vec[insertPosition.index]=item;
    length++;

  }

  value_type popFirst()
  {
    if (isEmpty())
    throw std::logic_error("logic_error");
    value_type popped=vec[0];
    erase(begin());
    return popped;
  }

  value_type popLast()
  {
    if (isEmpty())
    throw std::logic_error("logic_error");
    value_type popped=vec[length-1];
    erase(--end());
    return popped;
  }

  void erase(const const_iterator& possition)
  {
    if(possition==end())
    throw std::out_of_range("out_of_range");
    for(int i=possition.index;i<length;i++)
        vec[i]=vec[i+1];
    length--;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    int range=lastExcluded.index-firstIncluded.index;
    if(range<0)
    throw std::out_of_range("out_of_range");
    for(int i=firstIncluded.index;i+range<length;i++)
        vec[i]=vec[i+range];
    length-=range;
  }

  iterator begin()
  {
    return iterator(this,0);
  }

  iterator end()
  {
    return iterator(this,length);
  }

  const_iterator cbegin() const
  {
    return const_iterator(this,0);
  }

  const_iterator cend() const
  {
    return const_iterator(this,length);
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }



};

template <typename Type>
class Vector<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;

private:
  const Vector<Type>* v;
  int index;
  friend void Vector<Type>::erase(const const_iterator&);
  friend void Vector<Type>::erase(const const_iterator&, const const_iterator&);
  friend void Vector<Type>::insert(const const_iterator&, const Type&);

public:
  explicit ConstIterator(const Vector<Type>* a=nullptr,int i=0): v(a), index(i)
  {}

  reference operator*() const
  {
    if(index==v->length)
    throw std::out_of_range("out_of_range");
    return v->vec[index];
  }

  ConstIterator& operator++()
  {
    if(index>=v->length)
    throw std::out_of_range("out_of_range");
    index++;
    return *this;
  }

  ConstIterator operator++(int)
  {
    auto temp=*this;
    operator++();
    return temp;
  }

  ConstIterator& operator--()
  {
    if(index<=0)
    throw std::out_of_range("out_of_range");
    index--;
    return *this;
  }

  ConstIterator operator--(int)
  {
    auto temp=*this;
    operator--();
    return temp;
  }

  ConstIterator operator+(difference_type d) const
  {
    if(index+d>v->length)
    throw std::out_of_range("out_of_range");
    return ConstIterator(v,index+d);
  }

  ConstIterator operator-(difference_type d) const
  {
    if(index-d<0)
    throw std::out_of_range("out_of_range");
    return ConstIterator(v,index-d);
  }

  bool operator==(const ConstIterator& other) const
  {
    return v==other.v && index==other.index;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return v!=other.v || index!=other.index;
  }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;

  explicit Iterator(const Vector<Type>* a=nullptr,int i=0): ConstIterator(a,i)
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  Iterator operator+(difference_type d) const
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
    return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif // AISDI_LINEAR_VECTOR_H
