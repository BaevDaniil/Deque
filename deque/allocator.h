#pragma once

#include <cstdlib>
#include "allocator_interface.h"

/**
 * @brief Allocator for example
 *
 * This allocator based on standard malloc/free functions
 */
class my_allocator_t : public allocator_interface_t {
public:
  /**
   * A method that allocates a block of size 'size'
   *
   * @param[in] size of allocated memory in bytes
   * @return void * pointer to allocated memory
   */
  void* alloc(size_t size) override final {return malloc(size);}

  /**
   * Freeing a block by pointer
   *
   * @param[in] data pointer to block
   */
  void del(void* data) override final {free(data);}
};