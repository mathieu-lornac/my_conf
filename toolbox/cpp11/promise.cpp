// One shot event communication between 2 actors
// All the actors hold a reference to the promise
// The promise value is set by the detector
// The reactor waits for the value to be set


#include <iostream>
#include <future>
#include <thread>
#include <vector>
#include <unistd.h>


void detecting(std::promise<void> &promise) {
  sleep(3);
  std::cout << "--> Detected" << std::endl;
  promise.set_value();
}

void reacting(std::promise<void> &promise) {
  std::cout << "==> Waiting for the future to be available" << std::endl;
  promise.get_future().wait();
  std::cout << "==> Reacted" << std::endl;
}

int main() {

  {
    std::promise<void> promise;
    // Lauching both reacting and detecting
    auto det = std::async(std::launch::async, std::bind(detecting, std::ref(promise)));
    auto rea = std::async(std::launch::async, std::bind(reacting, std::ref(promise)));

    det.get();
    rea.get();
  }

}
