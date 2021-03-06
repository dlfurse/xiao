#ifndef HATCH_TREE_ITERATOR_IMPL_HH
#define HATCH_TREE_ITERATOR_IMPL_HH

#ifndef HATCH_TREE_HH
#error "do not include tree_iterator_impl.hh directly. include tree.hh instead."
#endif

namespace hatch {

  template<class T>
  tree_node<T>* tree_iterator<T>::_before =
      const_cast<tree_node<T>*>(reinterpret_cast<const tree_node<T>*>("tree_iterator::before"));

  template<class T>
  tree_node<T>* tree_iterator<T>::_after =
      const_cast<tree_node<T>*>(reinterpret_cast<const tree_node<T>*>("tree_iterator::after"));

  ///////////////////////////////////////////
  // Constructors, destructor, assignment. //
  ///////////////////////////////////////////

  template<class T>
  tree_iterator<T>::tree_iterator(tree<T>* owner, tree_node<T>* node) :
      owned<tree<T>, tree_iterator<T>>::owned{owner},
      _node{node} {
  }

  template<class T>
  tree_iterator<T>::tree_iterator() :
      owned<tree<T>, tree_iterator<T>>::owned{},
    _node{nullptr} {
  }

  template<class T>
  tree_iterator<T>::~tree_iterator() {
  }

  template<class T>
  tree_iterator<T>::tree_iterator(tree_iterator&& moved) noexcept :
      owned<tree<T>, tree_iterator<T>>::owned{std::move(moved)},
      _node{moved._node} {
    moved._node = nullptr;
  }

  template<class T>
  tree_iterator<T>& tree_iterator<T>::operator=(tree_iterator&& moved) noexcept {
    owned<tree<T>, tree_iterator<T>>::operator=(std::move(moved));
    _node = moved._node;
    moved._node = nullptr;
    return *this;
  }

  template<class T>
  tree_iterator<T>::tree_iterator(const tree_iterator& copied) :
      owned<tree<T>, tree_iterator<T>>::owned{copied},
      _node{copied._node} {
  }

  template<class T>
  tree_iterator<T>& tree_iterator<T>::operator=(const tree_iterator& copied) {
    owned<tree<T>, tree_iterator<T>>::operator=(copied);
    _node = copied._node;
    return *this;
  }

  //////////////////
  // Comparisons. //
  //////////////////

  template <class T>
  tree_iterator<T>::operator bool() const {
    return this->_owner;
  }

  template<class T>
  bool tree_iterator<T>::operator==(const tree_iterator& compared) const {
    return this->_owner == compared._owner && _node == compared._node;
  }

  template<class T>
  bool tree_iterator<T>::operator!=(const tree_iterator& compared) const {
    return this->_owner != compared._owner || _node != compared._node;
  }

  /////////////////////////////////////
  // Structure: get underlying data. //
  /////////////////////////////////////

  template<class T>
  T& tree_iterator<T>::operator*() const {
    return _node->get();
  }

  template<class T>
  T* tree_iterator<T>::operator->() const {
    return &_node->get();
  }

  ///////////////////////////////
  // Structure: move iterator. //
  ///////////////////////////////

  template<class T>
  tree_iterator<T>& tree_iterator<T>::operator++() {
    if (auto* tree = this->_owner) {
      if (_node == _before) {
        if (tree->_root == nullptr) {
          _node = _after;
        } else {
          _node = tree->minimum();
        }
      } else if (_node != _after) {
        _node = _node->successor();
        if (_node == nullptr) {
          _node = _after;
        }
      }
    }
    return *this;
  }

  template<class T>
  const tree_iterator<T>& tree_iterator<T>::operator++() const {
    return const_cast<tree_iterator<T>*>(this)->operator++();
  }

  template<class T>
  const tree_iterator<T> tree_iterator<T>::operator++(int) const {
    auto* const node = _node;
    this->operator++();
    return {this->_owner, node};
  }

  template<class T>
  tree_iterator<T>& tree_iterator<T>::operator--() {
    if (auto* tree = this->_owner) {
      if (_node == _after) {
        if (tree->_root == nullptr) {
          _node = _before;
        } else {
          _node = tree->maximum();
        }
      } else if (_node != _before) {
        _node = _node->predecessor();
        if (_node == nullptr) {
          _node = _before;
        }
      }
    }
    return *this;
  }

  template<class T>
  const tree_iterator<T>& tree_iterator<T>::operator--() const {
    return const_cast<tree_iterator<T>*>(this)->operator--();
  }

  template<class T>
  const tree_iterator<T> tree_iterator<T>::operator--(int) const {
    auto* const node = _node;
    this->operator--();
    return {this->_owner, node};
  }

  ////////////////////////////////////////
  // Structure: mutate underlying tree. //
  ////////////////////////////////////////

  template <class T>
  tree<T> tree_iterator<T>::remove() {
    if (auto* owner = this->_owner) {
      if (_node) {
        auto* data = this->_owner->_data;
        auto* other = _node->predecessor(data);
        if (!other) {
          other = _node->successor(data);
        }

        _node->remove(data);

        if (other) {
          owner->_root = other->root(data);
        } else {
          owner->_root = nullptr;
        }
        owner->disown_all();

        return tree<T, Ref>{_node, data};
      }
    }
    return tree<T, Ref>{};
  }

} // namespace hatch

#endif // HATCH_TREE_ITERATOR_IMPL_HH