/**
 ** Target: Being able to perform a selection on a compile time expression
 ** Specialisation on methods within a class
 ** Suppress some runtime overhead
 */

#include <iostream>
#include <string>

template <int v>
struct Int2Type {
  enum {
    value = v
  };
};


template <class T>
class Test {

public:

  // Different Class methods

  int convert(T v) {
    return convert(v, Int2Type<(sizeof(T) == 4)>());
  }

private:

  int convert(T v, Int2Type<true>) {
    std::cout << v << "True version" << std::endl;
    return 0;
  }

  int convert(T v, Int2Type<false>) {
    std::cout << v << "False version" << std::endl;
    return 1;
  }

};

int main() {
  Test<int> t;
  t.convert(12);
  return 0;
}
