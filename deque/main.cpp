#include "deque.h"
#include <iostream>
int main(void) {
  deque_t<int> deq;
  deq.PushBack(1);
  deq.PushBack(2);
  deq.PushBack(3);
  for (auto el = deq.rbegin(); el != deq.rend(); el++) {
    std::cout << *el << std::endl;
  }
  return 0; 
}