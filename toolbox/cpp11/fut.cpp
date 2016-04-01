// Task based programming approach
// Huge interest. No thread polling, better utilisation of the software threads
// Do not use if you need a thread priority policy
//
// std::async lauches a task in the current thread or in another thread or in a lazy mode (defered)
// Waiting for the task to be ready can be problematic. One needs to pay attention to the future status that can be deferred if nobody explicitely asked for the resolution of the future. i.e a call to get or to wait

#include <iostream>
#include <future>
#include <thread>
#include <vector>
#include <unistd.h>

// Tests if a task is ready
template<typename R>
bool is_ready(std::future<R> const& f)
{ return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }

void padawan() {
  // Lambda that performs a sleep
  auto lb = [](int sec) -> int {
    std::this_thread::sleep_for(std::chrono::seconds(sec));
    return sec;
  };

  // Lambda that prints an int
  auto res = [](int sec) {
    std::cout << "Result: " << sec << std::endl;
  };


  // Launching the async work
  auto fut1 = std::async(std::launch::async, std::bind(lb, 7));
  auto fut2 = std::async(std::launch::async, std::bind(lb, 5));
  auto fut3 = std::async(std::launch::async, std::bind(lb, 3));

  std::vector<std::future<int> *> futs = {&fut1, &fut2, &fut3};
  while (!futs.empty()) {
    auto it = futs.begin(), ite = futs.end();
    while (it != ite) {
      // Check if the element is ready
      if (is_ready(**it)) {
	res((*it)->get());
	futs.erase(it);
	break;
      }
      ++it;
    }
    usleep(1000);
  }
}


// Async work
void async() {

  // Lambda that performs a sleep
  auto lb = [](int sec) -> int {
    std::cout << "Async work of: " << sec << " seconds" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(sec));
    std::cout << "work done" << std::endl;
    return sec;
  };

  // Launching the async work
  std::future<int> fut1 = std::async(std::launch::async, std::bind(lb, 7));
  std::future<int> fut2 = std::async(std::launch::async, std::bind(lb, 5));
  // Lazy work of 3 seconds
  std::future<int> fut3 = std::async(std::launch::deferred, std::bind(lb, 5));

  // Extraction of the results
  int res1 = fut1.get();
  int res2 = fut2.get();
  std::cout <<  "Got res1: " << res1 << std::endl;
  std::cout <<  "Got res2: " << res2 << std::endl;
  std::cout << "Calling lazy eval fut3" << std::endl;
  int res3 = fut3.get();
  std::cout <<  "Got res3: " << res3 << std::endl;
}


int main() {
  padawan();
  async();
}
