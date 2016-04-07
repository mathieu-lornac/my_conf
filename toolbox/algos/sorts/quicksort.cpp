// C++ 11 implementation of the quicksort
// Pseudo algorithm
// * to sort the array arr, pick an element called the pivot
// * Run through the array in order to obtain all the elements lower or equal to the pivot to it's left
// all the higher elements to it's right. idx is the index of the pivot
// * Recursive calls on sub arrays until len of subarrays < 2

#include <iostream>
#include <stdlib.h>
#include <future>
#include <thread>
#include <time.h>
#include <vector>

template <class T>
void print_array(T lower, T upper) {
  std::cout << "Array";
  while (lower <= upper) {
    std::cout << " " << *lower;
    ++lower;
  }
  std::cout << std::endl;
}

template <class T>
T partition(T itl, T itu) {

  auto smaller = itl;
  auto greater = itu;
  typename T::value_type pivot = *greater;
  while (smaller < greater) {
    auto current = greater - 1;
    if (*current > pivot) {
      std::swap(*current, *greater);
      --greater;
    } else {
      typename T::value_type tmp = *smaller;
      *smaller = *current;
      *current = tmp;
      ++smaller;
    }
  }
  *greater = pivot;
  return greater;
}

void quicksort(std::vector<int>::iterator itl,
	       std::vector<int>::iterator itu) {
  if ((itu - itl) < 1) {
    return;
  } else if ((itu - itl) == 1) {
    if (*itl > *itu) {
      std::swap(*itl, *itu);
    }
    return;
  }
  auto &&pivot = partition(itl, itu);
  std::future<void> lowerSort = std::async(std::launch::async, std::bind(quicksort, itl, pivot - 1));
  std::future<void> upperSort = std::async(std::launch::async, std::bind(quicksort, pivot + 1, itu));
  lowerSort.wait();
  upperSort.wait();
}

int main() {

  std::vector<int> array;
  srand(time(NULL));

  std::cout << "Array to sort:";
  // Array initialisation
  for (int i = 0; i < 10 + rand() % 10; ++i) {
    array.push_back(rand() % 100);
    std::cout << " " << array.back();
  }
  std::cout << std::endl;

  quicksort(array.begin(), array.end() - 1);

  std::cout << "Sorted array:";
  for (auto && v : array) {
    std::cout << " " << v;
  }
  std::cout << std::endl;

}
