#pragma once
#include <initializer_list>

template <typename T>
class List {
 private:
  struct Node;

 public:
  List() {
    tail_ = new Node;
    head_ = tail_;
    size_ = 0;
  }

  ~List() {
    Node* cur_node = tail_;
    while (cur_node != nullptr) {
      Node* prev_node = cur_node->prev;
      delete cur_node;
      cur_node = prev_node;
    }
  }

  List(std::initializer_list<T> init_list) : List() {
    typename std::initializer_list<T>::iterator it = init_list.end();
    do { this->PushFront(*--it); } while (it != init_list.begin());
  }

  size_t GetSize() {
    return size_;
  }

  void PushFront(T val) {
    Node* new_node = new Node;
    new_node->next = head_;
    new_node->val = val;
    new_node->prev = nullptr;
    head_->prev = new_node;
    head_ = new_node;
    ++size_;
  }

  // void PushBack(T val) {
  //   assert(0);
  // }

  class iterator {
   public:
    iterator() {
      node_ptr_ = nullptr;
    }
    
    iterator(Node* node_ptr) {
      node_ptr_ = node_ptr;
    }

    Node* GetNode() {
      return node_ptr_;
    }

    iterator& operator++() {
      if (node_ptr_ != nullptr) {
        node_ptr_ = node_ptr_->next;
      }
      return *this;
    }

    iterator& operator--() {
      if (node_ptr_ != nullptr) {
        node_ptr_ = node_ptr_->prev;
      }
      return *this;
    }

    iterator operator++(int n) {
      iterator copy = *this;
      ++*this;
      return copy;
    }

    iterator Next() {
      iterator it(this->node_ptr_->next);
      return it;
    }

    T& operator*() {
      return node_ptr_->val;
    }

    bool operator==(const iterator& it) {
      return this->node_ptr_ == it.node_ptr_;
    }

    bool operator!=(const iterator& it) {
      return !(this->node_ptr_ == it.node_ptr_);
    }

    friend void List<T>::Pop(iterator& it);

   private:
    Node* node_ptr_ = nullptr;
  };

  void Pop(iterator& it) {
    $;
    Node* node = it.node_ptr_;
    assert(node->next != nullptr && "Pop of list.end()!");
    node->next->prev = node->prev;
    if (node->prev != nullptr) {
      node->prev->next = node->next;
    } else {
      head_ = node->next;
    }
    delete node;
    it.node_ptr_ = nullptr;
    --size_;
    $$;
  }

  void Dump() {
    printf("List dump:\n");
    printf("---------------------\n");
    printf("size = %lu\n", size_);
    Node* cur_node = head_;
    while (cur_node != nullptr) {
      printf("prev = %p\n", cur_node->prev);
      printf("node = %p\n", cur_node);
      printf("next = %p\n", cur_node->next);
      cur_node = cur_node->next;
      if (cur_node != nullptr) {
        printf("\n");
      }
    }
    printf("---------------------\n");
  }

  iterator begin() {
    return iterator(head_);
  }

  iterator end() {
    return iterator(tail_);
  }


 private:
  Node* head_ = nullptr;
  Node* tail_ = nullptr;
  size_t size_ = 0;

  struct Node {
    Node* next = nullptr;
    Node* prev = nullptr;
    T val;
  };
};