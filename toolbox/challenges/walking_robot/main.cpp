
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

int main(){
  std::cout << "Minimum time found using recursion: " << resolve(10) << std::endl;

  // If (value > c)
  // return 0

  std::cout << "Compile val: " << C<5>::cost << std::endl;
  return 0;
}
