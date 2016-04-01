/*
  Smart pointer framework

  std::auto_ptr => leftover of c++98
 */

#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

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

////////////////////////////////////////////////////////////////////
///  std::unique_ptr: exclusive ownership resource management	////
////////////////////////////////////////////////////////////////////
/*
Non null pointers own the data they point to
No copy allowed, move only
Destroys the data (delete op by default) on uniq_ptr destruction
*/

void unique_pointer() {
  {
    std::cout << "Uniqu factory instanciation " << std::endl;
    auto factory = []() { return std::unique_ptr<Foo>(new Foo()); };
    std::unique_ptr<Foo> f = factory();
    f->bar();
    // f owns the Foo object
    // call delete upon descoping
  }

  {
    std::cout << "Uniqu factory instanciation 2" << std::endl;
    auto factory = []() { return std::unique_ptr<Foo>(new Foo()); };
    auto &&f = factory();
    f->bar();
    // f owns the Foo object
    // call delete upon descoping
  }

  {
    std::cout << "Pushing to a collection (move calls)" << std::endl;
    auto factory = []() { return std::unique_ptr<Foo>(new Foo()); };
    auto &&f = factory();
    std::vector<std::unique_ptr<Foo> > v;
    v.push_back(factory());
    // Calling move to a rvalue and transfer the ownership to the collection
    v.push_back(std::move(f));
    assert(f.get() == nullptr);
  }
}

////////////////////////////////////////////////////////////////////////
///  Smart and weak pointers. Basically an import of the boost fwk////
////////////////////////////////////////////////////////////////////////

void shared_pointer() {
  // Instantiation of a shared
  auto sh = std::make_shared<Foo>();
  // Weak from this shared
  std::weak_ptr<Foo> wk(sh);

  // Direct dangle test
  assert(!wk.expired());

  // scope acquisition of the wk
  {
    auto lock = wk.lock();
    if (lock == nullptr) {
      // acquisition ok
    }
  }

  // scope acquisition form 2: Construction from the weak
  {
    try {
      std::shared_ptr<Foo> sh2(wk);
    } catch (std::bad_weak_ptr p) {
      // weak pointer was invalid
    }
  }

}

int main() {
  //  unique_pointer();
  shared_pointer();
}
