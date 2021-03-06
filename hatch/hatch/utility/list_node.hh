#ifndef HATCH_LIST_NODE_HH
#define HATCH_LIST_NODE_HH

#ifndef HATCH_LIST_HH
#error "do not include list_node.hh directly. include list.hh instead."
#endif

#include <hatch/utility/container.hh>
#include <hatch/utility/chain.hh>

namespace hatch {

  template <class T>
  class list_node :  public container<T>, public chain<list_node<T>> {
  public:
    friend class list<T>;
    friend class list_iterator<T>;

    ///////////////////////////////////////////
    // Constructors, destructor, assignment. //
    ///////////////////////////////////////////

  public:
    template <class ...Args>
    explicit list_node(Args&&... args);
    virtual ~list_node();

    list_node(list_node&& moved) noexcept;
    list_node& operator=(list_node&& moved) noexcept;

    list_node(const list_node&);
    list_node& operator=(const list_node&);
  };

} // namespace hatch

#endif // HATCH_LIST_NODE_HH
