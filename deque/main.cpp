#include "deque.h"

int main() {
  // default constructor
  deque_t<int> d1;
  std::cout << "default constructor: d1 = " << d1 << std::endl;

  // constructor with initializer list
  deque_t<int> d2({ 1, 2, 3, 4, 5});
  std::cout << "constructor with initializer list: d2 = " << d2 << std::endl;

  // copy constructor
  deque_t<int> d3(d2);
  std::cout << "copy constructor: d3 = " << d3 << std::endl;

  // move constructor
  deque_t<int> d4(deque_t<int>({ 1, 2, 3, 4, 5 }));
  std::cout << "move constructor: d4 = " << d4 << std::endl;

  // copy operator=
  deque_t<int> d5 = d2;
  std::cout << "copy operator=: d5 = " << d5 << std::endl;

  // move operator=
  deque_t<int> d6 = deque_t<int>({ 1, 2, 3, 4, 5 });
  std::cout << "move operator=: d6 = " << d6 << std::endl;

  //const iterarator
  std::cout << "const iterator for d2: ";
  for (deque_t<int>::const_iterator rev = d2.cbegin(); rev != d2.cend(); rev++)
    std::cout << *rev << " ";
  std::cout << std::endl << std::endl;

  //iterarator
  std::cout << "iterator for d2: ";
  for (deque_t<int>::iterator rev = d2.begin(); rev != d2.end(); rev++) {
    //*rev += 1;
    std::cout << *rev << " ";
  }
  std::cout << std::endl << std::endl;

  // reverse iterator
  std::cout << "reverse iterator for d2: ";
  for (deque_t<int>::reverse_iterator rev = d2.rbegin(); rev != d2.rend(); rev++) {
    //*rev -= 1;
    std::cout << *rev << " ";
  }
  std::cout << std::endl << std::endl;

  // Size
  std::cout << "Size of d2: " << d2.Size() << std::endl << std::endl;

  // PeekHead
  std::cout << "GetFront d2: " << d2.GetFront() << std::endl << std::endl;

  //PeekTail
  std::cout << "GetBack d2: " << d2.GetBack() << std::endl << std::endl;

  // PushFront with move semantic
  d2.PushFront(0);
  std::cout << "PushFront 0 into d2: d2 = " << d2 << std::endl;

  // PushBack with move semantic
  d3.PushBack(6);
  std::cout << "PushBack 6 into d3: d3 = " << d3 << std::endl;

  // PushFront with copy semantic
  int elem = 0;
  d4.PushFront(elem);
  std::cout << "PushFront elem = 0 into d4: d4 = " << d4 << std::endl;

  // PushBack with copy semantic
  d5.PushBack(elem);
  std::cout << "PushBack elem = 0 into d5: d5 = " << d5 << std::endl;

  // PopFront
  d6.PopFront();
  std::cout << "PopFront from d6: d6 = " << d6 << std::endl;

  // PopBack
  d2.PopBack();
  std::cout << "PopBack from d2: d2 = " << d2 << std::endl;

  // AddOtherDeque with move
  std::cout << "AddOtherDeque with move {1-5} + {1-5}: " << deque_t<int>({ 1, 2, 3, 4, 5 }).AddOtherDeque({ 1, 2, 3, 4, 5 }) << std::endl;

  // AddOtherDeque with copy
  std::cout << "AddOtherDeque with copy {1-5} + d2: " << deque_t<int>({ 1, 2, 3, 4, 5 }).AddOtherDeque(d2) << std::endl;

  // Clear and IsEmpty
  std::cout << "d2 IsEmpty: " << d2.IsEmpty() << std::endl;
  d2.Clear();
  std::cout << "d2 IsEmpty after Clear: " << d2.IsEmpty() << std::endl;

  return 0;
}