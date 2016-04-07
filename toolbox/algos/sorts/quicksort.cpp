// C++ 11 implementation of the quicksort

// Pseudo algorithm
// * Partition phase
// ** Pick a pivot (-> in this imple, last element)
// ** Reorganize the interval such that, the elements smaller are left to the pivot
// the greater elements are to the right of the table
// ** Pivot is at it's final place
// * Recursive calls on sub arrays until len of subarrays <= 2

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

/**
 * In place partitionning
 * Returns iterator on the pivot position
 * [lower elements][untreated][hole][higher elements]
 * untreated[0] -> smaller
 * untreated[-1] -> greater
 * * Init phase => pivot is set to the last element of the array
 *               greater points to "the hole"
 * * Iteration until smaller != greater
 *      current is set to the last untreated element
 *      if current > pivot:
 *          greater = current and --greater
 *      else:
 *          swap(first_untreated, current)
 *          ++smaller
*/
template <class T>
T partition(T itl, T itu) {

  auto smaller = itl;
  auto greater = itu;
  typename T::value_type pivot = *greater;
  while (smaller < greater) {
    auto current = greater - 1;
    if (*current > pivot) {
      *greater = *current;
      --greater;
    } else {
      std::swap(*smaller, *current);
      ++smaller;
    }
  }
  *greater = pivot;
  return greater;
}

/**
 * In place quicksort
 * Iterators call
 * itl -> first element of the collection
 * itu -> last element of the collection (not collection.end(), must point to the last element)
 */
template<class T>
void quicksort(T itl,
	       T itu) {
  if ((itu - itl) < 1) {
    return;
  } else if ((itu - itl) == 1) {
    if (*itl > *itu) {
      std::swap(*itl, *itu);
    }
    return;
  }
  auto &&pivot = partition(itl, itu);
  std::future<void> lowerSort = std::async(std::launch::async, std::bind(quicksort<T>, itl, pivot - 1));
  std::future<void> upperSort = std::async(std::launch::async, std::bind(quicksort<T>, pivot + 1, itu));
  lowerSort.wait();
  upperSort.wait();
}

/**
 * In place quicksort
 * Collection call
 */
template<class T>
void quicksort(T &array) {
  typename T::iterator itl = array.begin();
  typename T::iterator itu = array.end();
  quicksort<typename T::iterator>(itl, itu - 1);
}


int main() {

  std::vector<int> array;
  srand(time(NULL));

  std::cout << "Array to sort:";
  // Array initialisation
  for (int i = 0; i < rand() % 20; ++i) {
    array.push_back(rand() % 100);
    std::cout << " " << array.back();
  }
  std::cout << std::endl;

  quicksort(array);
  std::cout << "Sorted array:";
  for (auto && v : array) {
    std::cout << " " << v;
  }
  std::cout << std::endl;

}
