#pragma once

/**
 * @brief Allocator interface
 *
 * Base class for allocators
 */
class allocator_interface_t {
public:
  /**
   * A method that allocates a block of size 'size'
   *
   * @param[in] size of allocated memory in bytes
   * @return void* pointer to allocated memory
   */
  virtual void* alloc(size_t size) = 0;

  /**
   * Freeing a block by pointer
   *
   * @param[in] data pointer to block
   */
  virtual void del(void* data) = 0;
};