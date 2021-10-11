#pragma once

#include <iostream>
#include <exception>
#include "allocator.h"

/**
 * @brief Deque class
 * @tparam T type of stored elements
 * @tparam allocator the allocator to be used (an allocator based on malloc/free functions is used by default)
 * @warning for guaranteed compatibility, the allocator must be inherited from allocator_interface_t
 * @see allocator_interface_t
 * @see my_allocator_t
 */
template <typename T, typename allocator = my_allocator_t>
class deque_t {
private:
  /**
   * @brief deque node class
   *
   * The class node storing the value of the specified 'T' type
   */
  class node_t {
  public:
    T value;                  ///< stored value
    node_t* prev;             ///< pointer to previous element (nullptr if the node is the first)
    node_t* next;             ///< pointer to next element (nullptr if the node is the last)
  };

  node_t* head;               ///< pointer to the beginning of the deque (nullptr if the deque is empty)
  node_t* tail;                ///< pointer to the end of the deque (nullptr if the deque is empty)
  size_t size;                ///< size in elements in the deque

  allocator alloc;  ///< the allocator

public:
  /**
   * Stream output operator
   * @tparam T type of deque elements
   * @param[in] stream output stream
   * @param[in] deque deque to output
   * @return reference to stream
   */
  template <typename T>
  friend std::ostream& operator<<(std::ostream& stream, deque_t<T> const& deque);

  /**
   * @brief Deque iterator
   *
   * Allows to iterate in direct order in deque
   */
  class iterator {
  private:
    node_t* curNode;   ///< Pointer to the deque node to which the iterator corresponds
  public:
    /**
     * Default constructor for iterator
     */
    iterator() : curNode(head) {}

    /**
     * Constructor from pointer to the deque node
     * @param[in] node pointer to the deque node we want to build iterator from
     */
    iterator(node_t* node) : curNode(node) {}

    /**
     * Default copy constructor
     * @param[in] iter iterator to copy
     */
    iterator(iterator const& iter) = default;

    /**
     * Default move constructor
     * @param[in] iter iterator to move
     */
    iterator(iterator&& iter) = default;

    /**
     * Default copy operator
     * @param[in] iter iterator to copy
     * @return reference to iterator
     */
    iterator& operator=(iterator const& iter) = default;

    /**
     * Default move operator
     * @param[in] iter iterator to move
     * @return reference to iterator
     */
    iterator& operator=(iterator&& iter) = default;

    /**
     * Prefix ++ operator
     * @return reference to next iterator
     * @exception "Trying to use end iterator" if curNode is equal to nullptr
     */
    iterator& operator++() {
      if (curNode == nullptr)
        throw std::exception("Trying to use end iterator");

      curNode = curNode->next;

      return *this;
    }

    /**
     * Postfix ++ operator
     * @return reference to current iterator
     * @exception "Trying to use end iterator" if curNode is equal to nullptr
     */
    iterator operator++(int) {
      if (curNode == nullptr)
        throw std::exception("Trying to use end iterator");

      iterator tmp = *this;           //Create a copy

      curNode = curNode->next;

      return tmp;                     //Return a copy
    }

    /**
     * Prefix -- operator
     * @return reference to previous iterator
     * @exception "Trying to use begin iterator" if curNode is begin node
     */
    iterator& operator--() {
      if (curNode->prev == nullptr)
        throw exception("Trying to use begin iterator");

      if (curNode == nullptr)
        curNode = tail;
      else
        curNode = curNode->prev;

      return *this;
    }

    /**
     * Postfix -- operator
     * @return reference to current iterator
     * @exception "Trying to use begin iterator" if curNode is begin node
     */
    iterator operator--(int) {
      if (curNode->prev == nullptr)
        throw std::exception("Trying to use begin iterator");

      iterator tmp = *this;           //Create a copy

      if (curNode == nullptr)
        curNode = tail;
      else
        curNode = curNode->prev;

      return tmp;                     //Return a copy
    }

    /**
     * Comparison operator ==
     * @param[in] iter iterator we want to compare with
     * @return true if equals, false otherwise
     */
    bool operator==(iterator const& iter) const noexcept {
      return curNode == iter.curNode;
    }

    /**
     * Comparison operator !=
     * @param[in] iter iterator we want to compare with
     * @return false if equals, true otherwise
     */
    bool operator!=(iterator const& iter) const noexcept {
      return curNode != iter.curNode;
    }

    /**
     * Operator *
     * @return reference on value to which the iterator corresponds
     * @exception "Try to use end iterator" if curNode is equal to nullptr
     */
    T& operator*() const {
      if (curNode == nullptr)
        throw std::exception("Try to use end iterator");

      return curNode->value;
    }
  };

  /**
   * Method begin for iterator
   * @return iterator that corresponds first element
   */
  iterator begin() const {
    return iterator(head);
  }

  /**
   * Method end for iterator
   * @return iterator that corresponds element after last
   */
  iterator end() const {
    return iterator(nullptr);
  }

  /**
   * @brief Deque const iterator
   *
   * Allows to iterate in direct order in deque (does not allow changing elements)
   */
  class const_iterator {
  private:
    const node_t* curNode;   ///< Pointer to the deque const node to which the iterator corresponds
  public:
    /**
     * Default constructor for const iterator
     */
    const_iterator() : curNode(head) {}

    /**
     * Constructor from pointer to the deque node
     * @param[in] node pointer to the deque node we want to build iterator from
     */
    const_iterator(const node_t* node) : curNode(node) {}

    /**
     * Default copy constructor
     * @param[in] iter iterator to copy
     */
    const_iterator(const_iterator const& iter) = default;

    /**
     * Default move constructor
     * @param[in] iter iterator to move
     */
    const_iterator(const_iterator&& iter) = default;

    /**
     * Default copy operator
     * @param[in] iter iterator to copy
     * @return reference to iterator
     */
    const_iterator& operator=(const_iterator const& iter) = default;

    /**
     * Default move operator
     * @param[in] iter iterator to move
     * @return reference to iterator
     */
    const_iterator& operator=(const_iterator&& iter) = default;

    /**
     * Prefix ++ operator
     * @return reference to next iterator
     * @exception "Try to use end iterator" if curNode is equal to nullptr
     */
    const_iterator& operator++() {
      if (curNode == nullptr)
        throw std::exception("Try to use end iterator");

      curNode = curNode->next;

      return *this;
    }

    /**
     * Postfix ++ operator
     * @return reference to current iterator
     * @exception "Try to use end iterator" if curNode is equal to nullptr
     */
    const_iterator operator++(int) {
      if (curNode == nullptr)
        throw std::exception("Try to use end iterator");

      const_iterator tmp = *this;

      curNode = curNode->next;

      return tmp;
    }

    /**
     * Prefix -- operator
     * @return reference to previous iterator
     * @exception "Trying to use begin iterator" if curNode is begin node
     */
    const_iterator& operator--() {
      if (curNode->prev == nullptr)
        throw exception("Trying to use begin iterator");

      if (curNode == nullptr)
        curNode = tail;
      else
        curNode = curNode->prev;

      return *this;
    }

    /**
     * Postfix -- operator
     * @return reference to current iterator
     * @exception "Trying to use begin iterator" if curNode is begin node
     */
    const_iterator operator--(int) {
      if (curNode->prev == nullptr)
        throw exception("Trying to use begin iterator");

      const_iterator tmp = *this;           //Create a copy

      if (curNode == nullptr)
        curNode = tail;
      else
        curNode = curNode->prev;

      return tmp;                     //Return a copy
    }

    /**
     * Comparison operator ==
     * @param[in] iter iterator we want to compare with
     * @return true if equals, false otherwise
     */
    bool operator==(const_iterator const& iter) const noexcept {
      return curNode == iter.curNode;
    }

    /**
     * Comparison operator !=
     * @param[in] iter iterator we want to compare with
     * @return false if equals, true otherwise
     */
    bool operator!=(const_iterator const& iter) const noexcept {
      return curNode != iter.curNode;
    }

    /**
     * Operator *
     * @return reference on value to which the iterator corresponds
     * @exception "Try to use end iterator" if curNode is equal to nullptr
     */
    const T& operator*() const {
      if (curNode == nullptr)
        throw std::exception("Try to use end iterator");

      return curNode->value;
    }
  };

  /**
   * Method begin for iterator
   * @return iterator that corresponds first element
   */
  const_iterator cbegin() const {
    return const_iterator(head);
  }

  /**
   * Method end for iterator
   * @return iterator that corresponds element after last
   */
  const_iterator cend() const {
    return const_iterator(nullptr);
  }

  /**
   * @brief Deque reverse iterator
   *
   * Allows to iterate in reverse order in deque
   */
  class reverse_iterator {
  private:
    node_t* curNode;   ///< Pointer to the deque node to which the iterator corresponds
  public:
    /**
     * Default constructor for reverse iterator
     */
    reverse_iterator() : curNode(end) {}

    /**
     * Constructor from pointer to the deque node
     * @param[in] node pointer to the deque node we want to build iterator from
     */
    reverse_iterator(node_t* node) : curNode(node) {}

    /**
     * Default copy constructor
     * @param[in] iter iterator to copy
     */
    reverse_iterator(reverse_iterator const& iter) = default;

    /**
     * Default move constructor
     * @param[in] iter iterator to move
     */
    reverse_iterator(reverse_iterator&& iter) = default;

    /**
     * Default copy operator
     * @param[in] iter iterator to copy
     * @return reference to iterator
     */
    reverse_iterator& operator=(reverse_iterator const& iter) = default;

    /**
     * Default move operator
     * @param[in] iter iterator to move
     * @return reference to iterator
     */
    reverse_iterator& operator=(reverse_iterator&& iter) = default;

    /**
     * Prefix ++ operator
     * @return reference to next reverse iterator
     * @exception "Try to use end iterator" if curNode is equal to nullptr
     */
    reverse_iterator& operator++() {
      if (curNode == nullptr)
        throw std::exception("Try to use end iterator");

      curNode = curNode->prev;

      return *this;
    }

    /**
     * Postfix ++ operator
     * @return reference to current reverse iterator
     * @exception "Try to use end iterator" if curNode is equal to nullptr
     */
    reverse_iterator operator++(int) {
      if (curNode == nullptr)
        throw std::exception("Try to use end iterator");

      reverse_iterator tmp = *this;

      curNode = curNode->prev;

      return tmp;
    }

    /**
     * Prefix -- operator
     * @return reference to previous iterator
     * @exception "Trying to use begin iterator" if curNode is begin node
     */
    reverse_iterator& operator--() {
      if (curNode->next == nullptr)
        throw std::exception("Trying to use begin iterator");

      if (curNode == nullptr)
        curNode = head;
      else
        curNode = curNode->next;

      return *this;
    }

    /**
     * Postfix -- operator
     * @return reference to current iterator
     * @exception "Trying to use begin iterator" if curNode is begin node
     */
    reverse_iterator operator--(int) {
      if (curNode->next == nullptr)
        throw std::exception("Trying to use begin iterator");

      reverse_iterator tmp = *this;           //Create a copy

      if (curNode == nullptr)
        curNode = head;
      else
        curNode = curNode->next;

      return tmp;                     //Return a copy
    }

    /**
     * Comparison operator ==
     * @param[in] iter iterator we want to compare with
     * @return true if equals, false otherwise
     */
    bool operator==(reverse_iterator const& iter) const noexcept{
      return curNode == iter.curNode;
    }

    /**
     * Comparison operator !=
     * @param[in] iter iterator we want to compare with
     * @return false if equals, true otherwise
     */
    bool operator!=(reverse_iterator const& iter) const noexcept{
      return curNode != iter.curNode;
    }

    /**
     * Operator *
     * @return reference on value to which the iterator corresponds
     * @exception "Try to use end iterator" if curNode is equal to nullptr
     */
    T& operator*() const {
      if (curNode == nullptr)
        throw std::exception("Try to use end iterator");

      return curNode->value;
    }
  };

  /**
   * Method begin for reverse iterator
   * @return iterator that corresponds last element
   */
  reverse_iterator rbegin() {
    return reverse_iterator(tail);
  }

  /**
   * Method end for reverse iterator
   * @return iterator that corresponds element before first
   */
  reverse_iterator rend() {
    return reverse_iterator(nullptr);
  }

  /**
   * Default deque constructor
   */
  deque_t() : head(nullptr), tail(nullptr), size(0) {};

  /**
   * Deque constructor with initializer list
   * @param[in] list list of 'elemType' values
   */
  deque_t(std::initializer_list<T> list) : head(nullptr), tail(nullptr), size(0) {
    for (auto& i : list)
      PushBack(i);
  };

  /**
   * Copy constructor
   * @param[in] deque const reference on deque to copy
   */
  deque_t(deque_t const& deque) : head(nullptr), tail(nullptr), size(0) {
    for (auto& i : deque)
      PushBack(i);
  };

  /**
   * Move constructor
   * @param[in] deque rvalue reference on deque to move
   */
  deque_t(deque_t&& deque) {
    head = deque.head;
    end = deque.end;
    size = deque.size;

    deque.head = nullptr;
    deque.end = nullptr;
    deque.size = 0;
  };

  /**
   * Copy operator=
   * @param[in] deque const reference on deque to copy
   */
  void operator=(deque_t const& deque) {
    Clear();

    for (auto& i : deque)
      PushBack(i);
  }

  /**
   * Move operator=
   * @param[in] deque rvalue reference on deque to move
   */
  void operator=(deque_t&& deque) {
    Clear();
    head = deque.head;
    end = deque.end;
    size = deque.size;

    deque.head = nullptr;
    deque.end = nullptr;
    deque.size = 0;
  }

  /**
   * Check is deque empty method
   * @return true if deque is empty, false otherwise
   */
  bool IsEmpty() const {
    return head == nullptr;
  }

  /**
   * Get deque size method
   * @return number of elements in the deque
   */
  size_t Size() const {
    return size;
  }

  /**
   * Method to get first element
   * @returns reference on element
   * @exception "Deque is empty" if deque is empty
   */
  T& GetFront() {
    if (!head)
      throw std::exception("Deque is empty");

    return head->value;
  }

  /**
   * Method to get last element
   * @returns reference on element
   * @exception "Deque is empty" if deque is empty
   */
  T& GetBack() {
    if (!tail)
      throw std::exception("Deque is empty");

    return tail->value;
  }

  /**
   * Method put element to begin of deque (copy semantics)
   * @param[in] elem const reference on element
   */
  void PushFront(T const& elem) {
    node_t* tmp = (node_t*)alloc.alloc(sizeof(node_t));

    tmp->value = elem;
    tmp->next = head;
    tmp->prev = nullptr;

    if (head)
      head->prev = tmp;

    head = tmp;

    if (tail == nullptr)
      tail = head;

    size++;
  }

  /**
   * Method put element to begin of deque (move semantics)
   * @param[in] elem rvalue reference on element
   */
  void PushFront(T&& elem) {
    node_t* tmp = (node_t*)alloc.alloc(sizeof(node_t));

    tmp->value = std::move(elem);
    tmp->next = head;
    tmp->prev = nullptr;

    if (head)
      head->prev = tmp;

    head = tmp;

    if (tail == nullptr)
      tail = head;

    size++;
  }

  /**
   * Method put element to end of deque (copy semantics)
   * @param[in] elem const reference on element
   */
  void PushBack(T const& elem) {
    node_t* tmp = (node_t*)alloc.alloc(sizeof(node_t));

    tmp->value = elem;
    tmp->next = nullptr;
    tmp->prev = tail;

    if (tail)
      tail->next = tmp;

    tail = tmp;

    if (head == nullptr)
      head = tail;

    size++;
  }

  /**
   * Method put element to end of deque (move semantics)
   * @param[in] elem rvalue reference on element
   */
  void PushBack(T&& elem) {
    node_t* tmp = (node_t*)alloc.alloc(sizeof(node_t));

    tmp->value = std::move(elem);
    tmp->next = nullptr;
    tmp->prev = tail;

    if (tail)
      tail->next = tmp;

    tail = tmp;

    if (head == nullptr)
      head = tail;

    size++;
  }

  /**
   * Method to remove first element from deque
   * @exception "Deque is empty" if deque is empty
   */
  void PopFront() {
    if (head == nullptr)
      throw std::exception("Deque is empty");

    node_t* tmp = head;

    head = head->next;
    alloc.del((void*)tmp);

    if (head == nullptr)
      tail = nullptr;
    else
      head->prev = nullptr;

    size--;
  }

  /**
   * Method to remove last element from deque
   * @exception "Deque is empty" if deque is empty
   */
  void PopBack() {
    if (tail == nullptr)
      throw std::exception("Deque is empty");

    node_t* tmp = tail;

    tail = tail->prev;
    alloc.del((void*)tmp);

    if (tail == nullptr)
      head = nullptr;
    else
      tail->next = nullptr;

    size--;
  }

  /**
   * Method for adding elements of another deque to the end of the current one (copy semantics)
   * @param[in] deque const reference on other deque
   * @returns refernece on current deque
   */
  deque_t& AddOtherDeque(deque_t const& deque) {
    for (auto& d : deque)
      PushBack(d);

    return *this;
  }

  /**
   * Method for adding elements of another deque to the end of the current one (move semantics)
   * @param[in] deque rvalue reference on other deque
   * @returns refernece on current deque
   */
  deque_t& AddOtherDeque(deque_t&& deque) {
    if (tail == nullptr) {
      head = deque.head;
     tail = deque.tail;
      size = deque.size;

      deque.head = nullptr;
      deque.tail = nullptr;
      deque.size = 0;
    }
    else {
      if (deque.head) {
        tail->next = deque.head;
        deque.head->prev = tail;
        tail = deque.tail;
        size += deque.size;

        deque.head = nullptr;
        deque.tail = nullptr;
        deque.size = 0;
      }
    }

    return *this;
  }

  /**
   * Clear deque
   */
  void Clear(void) {
    while (head)
      PopBack();
  }

  /**
   * Deque destructor
   */
  ~deque_t() {
    Clear();
  }
};

/**
 * Operator<< for deque
 * @tparam type type of deque elements
 * @param[in] stream output stream
 * @param[in] deque deque to output
 * @return reference to stream
 */
template <typename type>
std::ostream& operator<<(std::ostream& stream, deque_t<type> const& deque) {
  for (auto& d : deque)
    std::cout << d << " ";
  std::cout << std::endl;

  return stream;
}