#pragma once

#include <iostream>
#include <stdexcept>
#include <exception>
#include <memory>
#include <type_traits>

/**
 * @brief Deque class
 * @tparam T type of stored elements
 * @tparam allocator the allocator to be used
 */
template <typename T, typename Allocator = std::allocator<T>>
class deque_t {
private:
  /**
   * @brief Deque node class
   *
   * The class node storing the value of the specified 'T' type
   */
  class node_t {
  public:
    T data;                   ///< stored value
    node_t* prev;             ///< pointer to previous element (nullptr if the node is the first)
    node_t* next;             ///< pointer to next element (nullptr if the node is the last)
  };

private:
  node_t* head;               ///< pointer to the beginning of the deque (nullptr if the deque is empty)
  node_t* tail;               ///< pointer to the end of the deque (nullptr if the deque is empty)
  size_t size;                ///< size in elements in the deque

  /**
   * @brief Deque iterator class
   * @tparam IsConst const's of this iterator
   */
  template<bool IsConst>
  class common_iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
    friend class deque_t;

  private:
    node_t* data;               ///< pointer to node in deque
    node_t* head;               ///< pointer to the beginning of the deque (nullptr if the deque is empty)
    node_t* tail;               ///< pointer to the end of the deque (nullptr if the deque is empty)
  private:
    /**
     * @brief Constructor from pointer to data
     * @param[in] data pointer in deque list
     */
    common_iterator(node_t* data, node_t* head, node_t* tail) : data(data), head(head), tail(tail) {};

    /**
     * @brief Default constructor
     */
    common_iterator() = default;
  public:
    /**
     * @brief Copy constructor
     * @param[in] other iterator to copy
     */
    common_iterator(common_iterator const& other) : data(other.data), head(other.head), tail(other.tail) {};

    /**
     * @brief Copy assignment operator
     * @param[in] other iterator to copy
     * @return reference to this iterator
     */
    common_iterator& operator=(common_iterator const& other) { 
      data = other.data;
      head = other.head;
      tail = other.tail;
      return *this;
    }

    /**
     * @brief Dereference operator *
     * @return reference (const reference for const iterator) to the pointed-to element
     */
    std::conditional_t<IsConst, T const&, T&> operator*() const {
      return data->data;
    }

    /**
     * @brief Dereference operator ->
     * @return pointer (const pointer for const iterator) to the pointed-to element
     */
    std::conditional_t<IsConst, T const*, T*> operator->() const {
      return data;
    }

    /**
     * @brief Prefix increment
     * @return reference to this iterator
     */
    common_iterator& operator++() {
      if (this == nullptr)
        data = head;
      else
        data = data->next;
      return *this;
    }

    /**
     * @brief Postfix increment
     * @return previous value of this iterator
     */
    common_iterator& operator++(int) {
      common_iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    /**
     * @brief Prefix decrement
     * @return reference to this iterator
     */
    common_iterator& operator--() {
      if (data == nullptr)
        data = tail;
      else
        data = data->prev;
      return *this;
    }

    /**
     * @brief Postfix decrement
     * @return previous value of this iterator
     */
    common_iterator& operator--(int) {
      common_iterator tmp = *this;
      --(*this);
      return tmp;
    }

    /**
     * @brief Equality operator
     * @param[in] other iterator to compare
     * @return true if iterators point to the same element else false
     */
    bool operator==(common_iterator const& other) const {
      return this->data == other.data;
    }

    /**
     * @brief Inequality operator
     * @param[in] other iterator to compare
     * @return true if iterators point to the different elements else false
     */
    bool operator!=(common_iterator const& other) const {
      return !(this->data == other.data);
    }
  };


private:
  using alloc_traits = std::allocator_traits<Allocator>;  //allocator traits

  template <typename T>
  using node_allocator = typename alloc_traits::template rebind_alloc<node_t>;

  using node_allocator_traits = typename alloc_traits::template rebind_traits<node_t>;

  Allocator alloc;                  ///< the allocator for T
  node_allocator<T> nodeAlloc;      ///< the allocator for node_t

public:
  /**
   * @brief Constructor of empty deque
   * @param[in] alloc allocator to use in deque
   */
  deque_t(Allocator const& alloc = Allocator()) : head(nullptr), tail(nullptr), size(0), alloc(alloc), nodeAlloc(alloc) {};

  /**
   * @briefCopy constructor
   * @param[in] other deque to copy
   */
  deque_t(deque_t const& deque) {
    alloc = deque.alloc;
    nodeAlloc = deque.nodeAlloc;
    for (auto& d : deque)
      PushBack(d);
  }

  /**
   * @briefMove constructor
   * @param[in] other deque to move
   */
  deque_t(deque_t&& deque) {
    alloc = deque.alloc;
    nodeAlloc = deque.nodeAlloc;
    head = deque.head;
    tail = deque.tail;
    size = deque.size;

    deque.head = nullptr;
    deque.tail = nullptr;
    deque.size = 0;
  }

  /**
   * @briefCopy assigment operator
   * @param[in] other deque to copy
   * @return reference to this deque
   */
  void operator=(deque_t const& deque) {
    Clear();

    alloc = deque.alloc;
    nodeAlloc = deque.nodeAlloc;
    for (auto& d : deque)
      PushBack(d);
  }

  /**
   * @briefMove assigment operator
   * @param[in] other deque to move
   * @return reference to this deque
   */
  void operator=(deque_t&& deque) {
    Clear();
    alloc = deque.alloc;
    nodeAlloc = deque.nodeAlloc;
    head = deque.head;
    tail = deque.tail;
    size = deque.size;

    deque.head = nullptr;
    deque.tail = nullptr;
    deque.size = 0;
  }

  /**
   * @brief Check is deque empty method
   * @return true if deque is empty, false otherwise
   */
  bool IsEmpty() const {
    return head == nullptr;
  }

  /**
   * @brief Get deque size method
   * @return number of elements in the deque
   */
  size_t Size() {
    return size;
  }

  /**
   * @brief Method put element to end of deque
   * @param[in] value element to add
   */
  template <typename T>
  void PushBack(T&& data) {
    try {
      node_t* newNode = node_allocator_traits::allocate(nodeAlloc, 1);
      newNode->data = data;
      newNode->prev = tail;
      newNode->next = nullptr;

      if (tail)
        tail->next = newNode;

      tail = newNode;

      if (head == nullptr)
        head = tail;

      ++size;
    }
    catch (std::bad_alloc){
      throw;
    }
  }

  /**
   * @brief Method put element to begin of deque
   * @param[in] value element to add
   */
  template <typename T>
  void PushFront(T&& data) {
    try {
      node_t* newNode = node_allocator_traits::allocate(nodeAlloc, 1);
      newNode->data = data;
      newNode->next = head;
      newNode->prev = nullptr;

      if (head)
        head->prev = newNode;

      head = newNode;

      if (tail == nullptr)
        tail = head;

      ++size;
    }
    catch (std::bad_alloc) {
      throw;
    }
  }

  /**
   * @brief Remove element from the back of deque
   */
  void PopBack() {
    if (tail == nullptr)
      return;

    node_t* oldTail = tail;
    tail = tail->prev;
    node_allocator_traits::deallocate(nodeAlloc, oldTail, 1);

    if (tail == nullptr)
      head = nullptr;
    else
      tail->next = nullptr;

    --size;
  }

  /**
   * @brief Remove element from the front of deque
   */
  void PopFront() {
    if (head == nullptr)
      return;

    node_t* oldHead = head;
    head = head->next;
    node_allocator_traits::deallocate(nodeAlloc, oldHead, 1);

    if (head == nullptr)
      tail = nullptr;
    else
      head->prev = nullptr;

    --size;
  }

  using iterator = common_iterator<false>;
  using const_iterator = common_iterator<true>;

  /*
   * @brief Begin of deque
   * @return iterator pointed to the first element of deque
   */
  iterator begin() const {
    return iterator(head, head, tail);
  }

  /*
   * @brief End of deque
   * @return iterator pointed to the next after last element of deque
   */
  iterator end() const {
    return iterator(tail->next, head, tail);
  }

  std::reverse_iterator<iterator> rbegin() {
    return (std::reverse_iterator<iterator>)end();
  }

  std::reverse_iterator<iterator> rend() {
    return (std::reverse_iterator<iterator>)begin();
  }

  /*
   * @brief Begin of deque
   * @return const iterator pointed to the first element of deque
   */
  const_iterator cbegin() const noexcept {
    return const_iterator(head, head, tail);
  }

  /*
   * @brief End of deque
   * @return const iterator pointed to the next after last element of deque
   * @warning dereferencing can cause undefined behaviour
   */
  const_iterator cend() const noexcept {
    return const_iterator(tail->next, head, tail);
  }

  /**
   * @brief Clear deque
   */
  void Clear() {
    while (head != nullptr)
      PopBack();
  }

  /**
   * @brief Deque destructor
   */
  ~deque_t() {
    Clear();
  }
};