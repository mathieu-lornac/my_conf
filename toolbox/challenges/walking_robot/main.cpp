
#include <iostream>

#define NB 5
int gains[NB] = {10, 5, 3, 2, 1};
int costs[NB] = {3, 4, 5, 7, 12};


int resolve(int distance_to_go, int cost = 0) {
  if (!distance_to_go)
    return cost - 2;
  for (int i = 0; i < NB; ++i)
    if (distance_to_go >= gains[i])
      {
	std::cout << "I: " << i << std::endl;
	return resolve(distance_to_go - gains[i], cost + costs[i]);
      }
  return cost;
}

//////


template <int e, bool i>
struct C;


template <bool i>
struct C<0, i>{
  enum {
    lessTen = 0,
    cost = 0
  };
};

template <int e>
struct C<e, false>{
  enum {
    cost = 0
  };
};


template <int e, bool iterate = true>
struct C {
  enum {
    cost = ((e >= 10) ? (3 + C<e - 10, (e >= 0)>::cost) :  ( (e >= 5) ? (4 + C<e - 5, (e >= 0) >::cost) :
							     ((e >= 3) ? (5 + C<e - 3, (e >= 0) >::cost) :
							      ((e >= 2) ? (7 + C<e - 2, (e >= 0) >::cost) :
							      ((e >= 1) ? (12 + C<e - 1, (e >= 0) >::cost) : 0
							       )
							       )
							      )
							     )
	    )
  };

};


#define VAL 14
int main(){

  std::cout << "Minimum time found using recursion: " << resolve(VAL) << std::endl;

  // If (value > c)
  // return 0

  std::cout << "Compile val: " << C<VAL>::cost - 2<< std::endl;
  return 0;
}
