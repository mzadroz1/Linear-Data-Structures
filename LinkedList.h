#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class LinkedList
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

  struct Node
  {
    pointer data;
    Node *prev;
    Node *next;
    Node()
    {
        data=nullptr;
        prev=nullptr;
        next=nullptr;
    }
    Node(value_type object):Node()
    {
        data=new value_type(object);
    }
    ~Node()
    {
        delete data;
    }
  };

private:
  Node *head;
  Node *tail;
  size_type length;

public:

  LinkedList()
  {
    head=new Node;
    tail=new Node;
    length=0;
    head->next=tail;
    head->prev=nullptr;
    tail->prev=head;
    tail->next=nullptr;
  }

  LinkedList(std::initializer_list<Type> l):LinkedList()
  {
    for(auto it=l.begin();it!=l.end();++it)
        append(*it);
  }

  LinkedList(const LinkedList& other):LinkedList()
  {
    for(auto it=other.begin();it!=other.end();it++)
        append(*it);
  }

  LinkedList(LinkedList&& other):LinkedList()
  {
    head=other.head;
    tail=other.tail;
    length=other.length;
    other.head=nullptr;
    other.tail=nullptr;
    other.length=0;
  }

  ~LinkedList()
  {
    while(!isEmpty())
        erase(begin());
    delete head;
    delete tail;
  }


  bool operator==(const LinkedList& other) const
  {
    if(head==other.head && tail==other.tail && length==other.length)
        return true;
    return false;
  }

  LinkedList& operator=(const LinkedList& other)
  {
    if(*this==other) return *this;

    erase(begin(),end());
    for (auto it=other.begin();it!=other.end();++it)
        append(*it);

    return *this;
  }

  LinkedList& operator=(LinkedList&& other)
  {
    erase(begin(),end());

    head=other.head;
    tail=other.tail;
    length=other.length;
    other.head=nullptr;
    other.tail=nullptr;
    other.length=0;

    return *this;
  }

  bool isEmpty() const
  {
    if(length==0)
        return true;
    return false;
  }

  size_type getSize() const
  {
    return length;
  }

  void append(const Type& item)
  {
    Node *temp=new Node(item);
    temp->prev=tail->prev;
    temp->next=tail;
    tail->prev->next=temp;
    tail->prev=temp;
    ++length;
  }

  void prepend(const Type& item)
  {
    Node *temp=new Node(item);
    temp->prev=head;
    temp->next=head->next;
    head->next->prev=temp;
    head->next=temp;
    ++length;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
      Node* temp=new Node(item);
      temp->next=insertPosition.ptr;
      temp->prev=insertPosition.ptr->prev;
      insertPosition.ptr->prev->next=temp;
      insertPosition.ptr->prev=temp;
      ++length;
  }

  value_type popFirst()
  {
    if(isEmpty())
      throw std::logic_error("logic_error");

    value_type popped=*begin();
    erase(begin());
    return popped;
  }

  value_type popLast()
  {
    if(isEmpty())
      throw std::logic_error("logic_error");

    value_type popped=*(--end());
    erase((--end()));
    return popped;
  }

  void erase(const const_iterator& position)
  {
    if(position==end())
      throw std::out_of_range("out_of_range");
    position.ptr->next->prev=position.ptr->prev;
    position.ptr->prev->next=position.ptr->next;
    delete position.ptr;
    --length;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    auto it=firstIncluded;
    while(it!=lastExcluded)
    {
      auto it1=it+1;
      erase(it);
      it=it1;
    }
  }

  iterator begin()
  {
    return iterator(head->next);
  }

  iterator end()
  {
    return iterator(tail);
  }

  const_iterator cbegin() const
  {
    return const_iterator(head->next);
  }

  const_iterator cend() const
  {
    return const_iterator(tail);
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
class LinkedList<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;

private:
  Node *ptr;
  friend void LinkedList <Type>::insert(const const_iterator&, const Type&);
  friend void LinkedList <Type>::erase(const const_iterator&);

public:

  explicit ConstIterator(Node *pnt=nullptr): ptr(pnt)
  {}

  reference operator*() const
  {
    if(ptr->data==nullptr)
        throw std::out_of_range("out_of_range");
    return *(ptr->data);
  }

  ConstIterator& operator++()
  {
    if (ptr->next==nullptr)
        throw std::out_of_range("out_of_range");
    ptr=ptr->next;
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
    if (ptr->prev->prev==nullptr)
        throw std::out_of_range("out_of_range");
    ptr=ptr->prev;
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
    auto temp=*this;
    for (difference_type i=0;i<d;++i)
        ++temp;
    return temp;
  }

  ConstIterator operator-(difference_type d) const
  {
    auto temp=*this;
    for (difference_type i=0;i<d;++i)
        --temp;
    return temp;
  }

  bool operator==(const ConstIterator& other) const
  {
    return this->ptr==other.ptr;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return this->ptr!=other.ptr;
  }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

  explicit Iterator(Node *pnt=nullptr): const_iterator(pnt)
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

#endif // AISDI_LINEAR_LINKEDLIST_H
