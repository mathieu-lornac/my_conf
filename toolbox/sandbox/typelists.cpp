/**
 * Playing with typelists
 */

typedef unsigned int uint;

// Regular nulltype struct
struct NullType{
};


// MACRO which simplifies declaration
#define TYPELIST_1(p) Typelist<p, NullType>
#define TYPELIST_2(p0, p1) Typelist<p0, TYPELIST_1(p1) >
#define TYPELIST_3(p0, p1, p2) Typelist<p0, TYPELIST_2(p1, p2) >

////////////////////////////////////////////
// The typelist structure
template<class U, class V>
struct Typelist{
  typedef U Head;
  typedef V Tail;
};


////////////////////////////////////////////
// Getting the length of a typelist
template<class U>
struct TLLength;

template<>
struct TLLength<NullType>{
  enum ev{
    length = 0
  };
};

template <class U, class V>
struct TLLength<Typelist<U, V> >{
  enum ev{
    length = 1 + TLLength<V>::length
  };
};


////////////////////////////////////////////
// Getting the type at a specified position
template <class T, uint pos>
struct TLTypeAt;

template <class T, class U>
struct TLTypeAt<Typelist<T, U>, 0>{
  typedef T Type;
};

template <class T, class U, uint pos>
struct TLTypeAt<Typelist<T, U>, pos>{
  typedef typename TLTypeAt<U, pos - 1>::Type Type;
};


////////////////////////////////////////////
// Getting the index matching a specific type
template <class TL, class ToFind>
struct TLIndexOf;

// Not Found
template <class ToFind>
struct TLIndexOf<NullType, ToFind> {
  enum e {
    index = -1
  };
};

// Have a match
template <class T, class Tail>
struct TLIndexOf<Typelist<T, Tail>, T> {
  enum e {
    index = 0
  };
};

// No match on this iteration
template <class T, class Tail, class ToFind>
struct TLIndexOf<Typelist<T, Tail>, ToFind> {
private:
  enum { pindex = TLIndexOf<Tail, ToFind >::index };

public:
  enum e {
    index = (pindex == -1) ? (-1) : (1 + pindex)
  };
};


#include <iostream>
#include <typeinfo>

#define TESTTL TYPELIST_3(int, short, long)

int main() {

  std::cout << "TL length:" << TLLength<TESTTL>::length << std::endl;
  std::cout << "TL TypeAt 0 (i):" << typeid(TLTypeAt<TESTTL, 0>::Type).name() << std::endl;
  std::cout << "TL TypeAt 1 (s):" << typeid(TLTypeAt<TESTTL, 1>::Type).name() << std::endl;
  std::cout << "TL TypeAt 2 (l):" << typeid(TLTypeAt<TESTTL, 2>::Type).name() << std::endl;

  std::cout << "TL IndexOf (int):" << TLIndexOf<TESTTL, int>::index << std::endl;
  std::cout << "TL IndexOf (short):" << TLIndexOf<TESTTL, short>::index << std::endl;
  std::cout << "TL IndexOf (long):" << TLIndexOf<TESTTL, long>::index << std::endl;
  std::cout << "TL IndexOf (char notinlist):" << TLIndexOf<TESTTL, char>::index << std::endl;


  return 0;
}
