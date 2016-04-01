// Rvalues, lvalues
// Move semantics

#include <iostream>
#include <memory>
#include <vector>

// lvalues and rvalues differences
// Basically an rvalue is something we cannot address directly via it's address
void rvalues() {
  // // lvalues:
  // int i = 42;
  // i = 43; // ok, i is an lvalue
  // int* p = &i; // ok, i is an lvalue
  // int& foo();
  // foo() = 42; // ok, foo() is an lvalue
  // int* p1 = &foo(); // ok, foo() is an lvalue
  // // rvalues:
  // int foobar();
  // int j = 0;
  // j = foobar(); // ok, foobar() is an rvalue
  // int* p2 = &foobar(); // error, cannot take the address of an rvalue
  // j = 42; // ok, 42 is an rvalue
}

////////////////////////////////////////////////////////////////////
///                  Universal references			////
////////////////////////////////////////////////////////////////////
template<typename T>
void uniref_test(T&& param);

/*
If uniref_test is called with an lvalue, param is resolved as an lvalue reference
If uniref_test is called with an rvalue, param is resolved as an rvalue ref
*/

////////////////////////////////////////////////////////////////////
///                   Move semantics basics			////
////////////////////////////////////////////////////////////////////

class Foo {

public:

  Foo() : data(new int[10]) {
    std::cout << "Foo()" << std::endl;
  }

  ~Foo() {
    std::cout << "~Foo()" << std::endl;
    delete [] data;
  }

  Foo(const Foo&rhs) : Foo() {
    std::cout << "Foo(const Foo&rhs)" << std::endl;
  }

  Foo(Foo &&rhs) : Foo() {
    std::cout << "Foo(Foo &&rhs)" << std::endl;
    std::swap(data, rhs.data);
  }

  Foo & operator=(Foo &&rhs) {
    std::cout << "operator=(Foo &&rhs)" << std::endl;
    std::swap(data, rhs.data);
    return *this;
  }

  Foo &bar() {
    std::cout << "Foo::bar()" << std::endl;
    return *this;
  }

  int *data;
};

Foo fooMaker() {
  Foo f;
  return f;
}

void move_semantics_basics() {
  {
    std::cout << "=== Creation of Foo instance ===" << std::endl;
    Foo f;
    f.bar();
    std::cout << "=== Calling fooMaker (assignment move) ===" << std::endl;
    // Foo(fooMaker) => non addressable -> rvalue
    f = Foo(fooMaker());
  }

  {
    std::cout << "=== Calling fooMaker (move ctor) ===" << std::endl;
    std::vector<Foo> v;
    v.push_back(Foo());
    std::cout << "=== lvalue -> Regular copy ===" << std::endl;
    Foo f;
    v.push_back(f);
  }
}

////////////////////////////////////////////////////////////////////
///                  Perfect forwarding				////
////////////////////////////////////////////////////////////////////

template<typename T, typename Arg>
std::shared_ptr<T> factory(Arg&& arg)
{
  return std::shared_ptr<T>(new T(std::forward<Arg>(arg)));
}

// Forward template definition
// template<class S>
// S&& forward(typename remove_reference<S>::type& a) noexcept
// {
//   return static_cast<S&&>(a);
// }

/*
Reference collapsing rules:
    A& & becomes A&
    A& && becomes A&
    A&& & becomes A&
    A&& && becomes A&&

template<typename T>
void foo(T&&);

Here, the following apply:
* When foo is called on an lvalue of type A, then T resolves to A& and hence, by the reference collapsing rules above, the argument type effectively becomes A&.
*  When foo is called on an rvalue of type A, then T resolves to A, and hence the argument type becomes A&&.
 */


void forward() {

  std::cout << "===Forward: lvalue call ====" << std::endl;
  Foo f;
  auto ptr = factory<Foo>(f);
  std::cout << "===Forward: rvalue call ====" << std::endl;
  auto ptr2 = factory<Foo>(Foo());

}

int main() {

  //  move_semantics_basics();
  forward();
  return 0;
}
