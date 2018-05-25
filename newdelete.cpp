
#include <cstdlib>
#include <stdio.h>
#include <new>

#include "log.h"


namespace my {

  std::shared_ptr<spdlog::logger> my_logger;

  std::size_t alloc_counter = 0;

  void* malloc(std::size_t size)  throw (std::bad_alloc)
  {
    void* p = std::malloc(size);
    // printf("malloc: %zu %p %zu\n", ++alloc_counter, p, size);
    if(my_logger.get() != nullptr)
      SPDLOG_TRACE(my_logger, "malloc: {} {:p} {}\n", ++alloc_counter, p, size);
    return p;
  }

  void free(void* p) noexcept
  {
    // printf("free: %zu %p\n", --alloc_counter, p);
    if(my_logger.get() != nullptr)
      SPDLOG_TRACE(my_logger, "free: {} {:p}", --alloc_counter, p);
    std::free(p);
    return;
  }
}

