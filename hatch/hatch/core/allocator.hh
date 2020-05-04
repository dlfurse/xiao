#ifndef HATCH_ALLOCATOR_HH
#define HATCH_ALLOCATOR_HH

#ifndef HATCH_MEMORY_HH
#error "do not include pointer.hh directly. include memory.hh instead."
#endif

#include <memory> // std::aligned_storage

#include <cstddef> // std::max_align_t, size_t
#include <cstdint> // uint64_t

namespace hatch {

  template <class T>
  class allocator;

  template <class T>
  class allocator {
  public:
    friend class pointer<T>;

  public:
    allocator(uint64_t min_capacity = 1024, uint8_t max_doubling = 8);
    ~allocator();

    template <class ...Args>
    pointer<T> create(Args&&... args);
    void destroy(pointer<T>& destroyed);

    uint64_t allocated() const {
      return _allocated;
    }

    uint64_t capacity() const {
      return _capacity;
    }

    uint64_t growth_increment() const {
      return _growth_increment;
    }

    uint64_t shrink_threshold() const {
      return _shrink_threshold;
    }

  private:
    class node {
    public:
      uint64_t prev;
      uint64_t next;
      const pointer<T>* created;
      typename std::aligned_storage<sizeof(T), alignof(T)>::type data;
    };

    node* _data;
    uint64_t _free;

    uint64_t _allocated;
    uint64_t _capacity;
    uint64_t _growth_increment;
    uint64_t _shrink_threshold;

    const uint64_t _min_capacity;
    const uint64_t _lin_boundary;

    static constexpr uint64_t align = alignof(std::max_align_t);
    static constexpr uint64_t size = sizeof(node);
  };
}

#endif // HATCH_ALLOCATOR_HH
