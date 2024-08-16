// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <cassert>

template< class T>
class List {

    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    struct Base_node {
        Base_node* prev;
        Base_node* next;

        virtual ~Base_node() = default;
    };

    struct Node : public Base_node {
        T value {};
        Node(const T& value) : value(value) {}
    };

    Base_node base_node_;
    size_type size_{};

    template <bool is_const>
    class base_iterator {
    public:
        //using iterator_category = std::bidirectional_iterator_tag;
        //using value_type = T;
        //using difference_type = std::ptrdiff_t;
        using pointer = typename std::conditional_t<is_const, const Node*, Node*>;
        using reference = typename std::conditional_t<is_const, const T&, T&>;

        pointer ptr_ = nullptr;

        base_iterator() : ptr_(nullptr) {}

        base_iterator(pointer ptr)
            : ptr_(ptr) {}

        base_iterator(const base_iterator& other) : ptr_(other.ptr_) {}

        base_iterator& operator=(const base_iterator& other) {
            ptr_ = other.ptr_;
            return *this;
        }

        reference operator*() const {
            return ptr_->value;
        }

        pointer operator->() const {
            return ptr_;
        }

        base_iterator& operator++() {
            ptr_ = static_cast<pointer>(ptr_->next);
            return *this;
        }

        base_iterator& operator--() {
            ptr_ = static_cast<pointer>(ptr_->prev);
            return *this;
        }

        base_iterator operator++(int) {
            base_iterator temp = *this;
            ++(*this);
            return temp;
        }

        base_iterator operator--(int) {
            base_iterator temp = *this;
            --(*this);
            return temp;
        }

        bool operator==(const base_iterator& other) const {
            return ptr_ == other.ptr_;
        }

        bool operator!=(const base_iterator& other) const {
            return ptr_ != other.ptr_;
        }

        operator base_iterator<true>() const {
            return base_iterator<true>(ptr_);
        }
    };
public:
    using iterator = base_iterator<false>;
    using const_iterator = base_iterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    List() {
        base_node_.next = &base_node_;
        base_node_.prev = &base_node_;
    }

    void push_back(const T& value) {
        Node* new_node = new Node(value);

        if (base_node_.next == &base_node_) {
            base_node_.next       =  new_node;
            base_node_.prev       =  new_node; 
            new_node->next        =  &base_node_;
            new_node->prev        =  &base_node_;
        }
        else {
            base_node_.prev->next = new_node;
            new_node->prev        = base_node_.prev;
            new_node->next        = &base_node_;
            base_node_.prev       = new_node;
        }

        ++size_;
    }

    void pop_back(const T& value) {

    }

    void print()
    {
        Base_node* node = base_node_.next;
        while (node != &base_node_) {
            Node* current_node = static_cast<Node*>(node);
            std::cout << current_node->value << std::endl;
            node = node->next;
        }
    }

    // Capacity
    bool empty() const {
        return size_ == 0;
    }
    size_type size() const {
        return size_;
    }
    void resize(size_type count) {
        if (count == size_) {
            return; // Размер уже правильный
        }
        else if (count > size_) {
            // Увеличение размера
            for (size_type i = size_; i < count; ++i) {
                push_back(T{}); // Добавляем элементы по умолчанию
            }
        }
        else {
            // Уменьшение размера
            Node* node = static_cast<Node*>(base_node_.prev);
            for (size_type i = size_; i > count; --i) {
                Node* to_delete = node;
                node = static_cast<Node*>(node->prev); // Перемещаемся к предыдущему узлу
                delete to_delete; // Освобождаем память
            }
            base_node_.prev = node;
            node->next = &base_node_;
            base_node_.next->prev = node;
        }
        size_ = count;
    }
    void resize(size_type count, const value_type& value) {

    }

    // Iterators
    iterator begin() {
        return iterator(static_cast<Node*>(base_node_.next));
    }

    const_iterator cbegin() const {
        return const_iterator(static_cast<const Node*>(base_node_.next));
    }

    iterator end() {
        return iterator(static_cast<Node*>(&base_node_));
    }

    const_iterator cend() const {
        return const_iterator(static_cast<const Node*>(&base_node_));
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(cend());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(cbegin());
    }

    // Element access
    reference front() {
        return static_cast<Node*>(base_node_.next)->value;
    }
    const_reference front() const {
        return static_cast<Node*>(base_node_.next)->value;
    }
    reference back() {
        return static_cast<Node*>(base_node_.prev)->value;
    }
    const_reference back() const {
        return static_cast<Node*>(base_node_.prev)->value;
    }

    //explicit list(const allocator_type& alloc);
    //explicit list(size_type count, const T& value = T(), const Allocator& alloc = Allocator());

    /*
    template< class InputIt >
    list(InputIt first, InputIt last, const Allocator& alloc = Allocator());
    list(const list& other);
    list(list&& other);
    list(std::initializer_list<T> init, const Allocator& alloc = Allocator());
    ~list();

    list& operator=(const list& other);
    list& operator=(list&& other);
    list& operator=(std::initializer_list<T> ilist);

    void assign(size_type count, const T& value);
    template< class InputIt >
    void assign(InputIt first, InputIt last);
    void assign(std::initializer_list<T> ilist);

    allocator_type get_allocator() const;

    // Modifiers
    void clear();
    iterator insert(const_iterator pos, const T& value);
    iterator insert(const_iterator pos, T&& value);
    iterator insert(const_iterator pos, size_type count, const T& value);
    template< class InputIt >
    iterator insert(const_iterator pos, InputIt first, InputIt last);
    iterator insert(const_iterator pos, std::initializer_list<T> ilist);
    template< class... Args >
    iterator emplace(const_iterator pos, Args&&... args);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    void push_back(const T& value);
    void push_back(T&& value);
    template< class... Args >
    reference emplace_back(Args&&... args);
    void pop_back();
    void push_front(const T& value);
    void push_front(T&& value);
    template< class... Args >
    reference emplace_front(Args&&... args);
    void pop_front();
    void swap(list& other);
    void merge(list& other);
    void merge(list&& other);
    template< class Compare >
    void merge(list& other, Compare comp);
    template< class Compare >
    void merge(list&& other, Compare comp);
    void splice(const_iterator pos, list& other);
    void splice(const_iterator pos, list&& other);
    void splice(const_iterator pos, list& other, const_iterator it);
    void splice(const_iterator pos, list&& other, const_iterator it);
    void splice(const_iterator pos, list& other, const_iterator first, const_iterator last);
    void splice(const_iterator pos, list&& other, const_iterator first, const_iterator last);
    void remove(const T& value);
    template< class UnaryPredicate >
    void remove_if(UnaryPredicate p);
    void reverse() noexcept;
    void unique();
    template< class BinaryPredicate >
    void unique(BinaryPredicate p);
    void sort();
    template< class Compare >
    void sort(Compare comp);

    // Operations
    template< class BinaryPredicate >
    bool operator==(const list& lhs, const list& rhs);
    template< class BinaryPredicate >
    bool operator!=(const list& lhs, const list& rhs);
    template< class BinaryPredicate >
    bool operator<(const list& lhs, const list& rhs);
    template< class BinaryPredicate >
    bool operator<=(const list& lhs, const list& rhs);
    template< class BinaryPredicate >
    bool operator>(const list& lhs, const list& rhs);
    template< class BinaryPredicate >
    bool operator>=(const list& lhs, const list& rhs);
    */
};

int main()
{
    List<int> list;
    list.push_back(15);
    list.push_back(13);
    list.push_back(12);
    list.push_back(11);

    List<int>::reverse_iterator it = list.rbegin();
    std::cout << *it << std::endl;

    std::cout << list.front() << std::endl;



    return 0;
}