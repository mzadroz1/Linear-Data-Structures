#include <cstddef>
#include <cstdlib>
#include <string>

#include<chrono>
#include<iostream>

#include "Vector.h"
#include "LinkedList.h"

namespace
{

template <typename T>
using LinkedList = aisdi::LinkedList<T>;
template <typename T>
using Vector = aisdi::Vector<T>;

void appendLinkedListTest()
{
  LinkedList<std::string> collection;
  collection.append("TODO");
}

void appendVectorTest()
{
  Vector<std::string> collection;
  collection.append("TODO");
}

void prependLinkedListTest()
{
  LinkedList<std::string> collection;
  collection.prepend("TODO");
}

void prependVectorTest()
{
  Vector<std::string> collection;
  collection.prepend("TODO");
}

void eraseLinkedListTest(std::size_t repeatCount)
{
    LinkedList<std::string> collection;

    for (std::size_t i = 0; i < repeatCount; ++i)
        collection.prepend("TODO");

    auto start = std::chrono::system_clock::now();
    for (std::size_t i = 0; i < repeatCount; ++i)
        collection.erase(collection.begin());
    auto done = std::chrono::system_clock::now();
    std::cout<<"LinkedList erase begin time: "<<(done-start).count()<<'\n';
}

void eraseVectorTest(std::size_t repeatCount)
{
    Vector<std::string> collection;
    for (std::size_t i = 0; i < repeatCount; ++i)
        collection.prepend("TODO");

    auto start = std::chrono::system_clock::now();
    for (std::size_t i = 0; i < repeatCount; ++i)
        collection.erase(collection.begin());
    auto done = std::chrono::system_clock::now();
    std::cout<<"Vector erase begin time: "<<(done-start).count()<<'\n';
}

void eraseLinkedListTest1(std::size_t repeatCount)
{
    LinkedList<std::string> collection;

    for (std::size_t i = 0; i < repeatCount; ++i)
        collection.prepend("TODO");

    auto start = std::chrono::system_clock::now();
    for (std::size_t i = 0; i < repeatCount; ++i)
        collection.erase(collection.end()-1);
    auto done = std::chrono::system_clock::now();
    std::cout<<"LinkedList erase end time: "<<(done-start).count()<<'\n';
}

void eraseVectorTest1(std::size_t repeatCount)
{
    Vector<std::string> collection;
    for (std::size_t i = 0; i < repeatCount; ++i)
        collection.prepend("TODO");

    auto start = std::chrono::system_clock::now();
    for (std::size_t i = 0; i < repeatCount; ++i)
        collection.erase(collection.end()-1);
    auto done = std::chrono::system_clock::now();
    std::cout<<"Vector erase end time: "<<(done-start).count()<<'\n';
}

} // namespace

int main(int argc, char** argv)
{
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;

  auto start = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < repeatCount; ++i)
    appendLinkedListTest();
  auto done = std::chrono::system_clock::now();
  std::cout<<"LinkedList append time: "<<(done-start).count()<<'\n';

  start = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < repeatCount; ++i)
    appendVectorTest();
  done = std::chrono::system_clock::now();
  std::cout<<"Vector append time: "<<(done-start).count()<<'\n';

  start = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < repeatCount; ++i)
    prependLinkedListTest();
  done = std::chrono::system_clock::now();
  std::cout<<"LinkedList prepend time: "<<(done-start).count()<<'\n';

  start = std::chrono::system_clock::now();
  for (std::size_t i = 0; i < repeatCount; ++i)
    prependVectorTest();
  done = std::chrono::system_clock::now();
  std::cout<<"Vector prepend time: "<<(done-start).count()<<'\n';

  eraseLinkedListTest(repeatCount);
  eraseVectorTest(repeatCount);
  eraseLinkedListTest1(repeatCount);
  eraseVectorTest1(repeatCount);

  return 0;
}
