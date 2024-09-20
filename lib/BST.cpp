#include <iostream>
#include <type_traits>
#include <memory>
#include <functional>
#include <iterator>
#include <limits>

struct inorder_tag {};
struct preorder_tag {};
struct postorder_tag {};

template <typename T, typename Compare = std::less<T>, typename Alloc = std::allocator<T>>
class BinarySearchTree {
private:
    struct Node; 

    struct BaseNode {
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;
    };

    struct Node: BaseNode {
        T data_;
        Node(const T& data) : data_(data) {}
    };

    BaseNode* fake_node_;
    BaseNode base_node_;
    std::allocator_traits<Alloc>::template rebind_alloc<Node> alloc_;
    Compare comp_;
    size_t size_;

    template <typename OrderType = inorder_tag>
    class base_iterator {
    friend BinarySearchTree;
    public:
        using pointer_type = const T*;
        using referense_type = const T&;
        using value_type = BaseNode;
        using difference_type = size_t;
        using key_type = const T;

        base_iterator(const base_iterator&) = default;
        base_iterator& operator=(const base_iterator&) = default;

        bool operator==(const base_iterator&) const = default;
        bool operator!=(const base_iterator&) const = default;

        referense_type operator*() const { return ((static_cast<Node*>(ptr_))->data_); }
        pointer_type operator->() const { return &(static_cast<Node*>(ptr_))->data_; }

        base_iterator& operator++() {
            return increment(OrderType{});
        }

        base_iterator& operator--() {
            return decrement(OrderType{});
        }

        base_iterator& increment(inorder_tag) {
            if (ptr_->right != nullptr) {
                ptr_ = ptr_->right;
                while (ptr_->left != nullptr) {
                    ptr_ = ptr_->left;
                }
                return *this;
            }
            Node* par = ptr_->parent;
            while ((par != ptr_) && (ptr_ == par->right)) {
                ptr_ = par;
                par = par->parent;
            }
            ptr_ = par;
            return *this;
        }

        base_iterator& increment(preorder_tag) {
            if (ptr_->left != nullptr) {
                ptr_ = ptr_->left;
            } else if (ptr_->right != nullptr) {
                ptr_ = ptr_->right;
            } else {
                Node* par = ptr_->parent; 
                while ((par->right == nullptr) || (par->right == ptr_)) {
                    ptr_ = par;
                    par = par->parent;
                }
                if (par->left == ptr_ && par->parent == par) {
                    ptr_ = par;
                    return *this;
                }
                ptr_ = par->right;
            }
            return *this;
        }

        base_iterator& decrement(preorder_tag) {
            if ((ptr_->parent->left == ptr_) || (ptr_->parent->right == ptr_ && ptr_->parent->left == nullptr)) {
                ptr_ = ptr_->parent;
            } else {
                ptr_ = ptr_->parent->left;
                while ((ptr_->left != nullptr) || (ptr_->right != nullptr)) {
                    if (ptr_->right != nullptr) {
                        ptr_ = ptr_->right;
                    } else {
                        ptr_ = ptr_->left;
                    }
                }
            }
            return *this;
        }

        base_iterator& increment(postorder_tag) {
            Node* par = ptr_->parent;
            if (!(par->right == nullptr || par->parent == par || par->right == ptr_)) {
                par = par->right;
                while (par->left != nullptr || par->right != nullptr) {
                    if (par->left != nullptr) {
                        par = par->left;
                    } else {
                        par = par->right;
                    }
                }
            }
            ptr_ = par;
            return *this;
        }

        base_iterator& decrement(postorder_tag) {
            if (ptr_->parent == ptr_) {
                ptr_ = ptr_->left;
                return *this;
            }
            if (ptr_->right != nullptr) {
                ptr_ = ptr_->right;
            } else if (ptr_->left != nullptr) {
                ptr_ = ptr_->left;
            } else {
                while ((ptr_->parent->left == nullptr) || (ptr_->parent->left == ptr_)) {
                    ptr_ = ptr_->parent;
                }
                ptr_ = ptr_->parent->left;
            }
            return *this;
        }

        base_iterator operator++(int) {
            auto copy = *this;
            ++ptr_;
            return copy;
        }

        base_iterator& decrement(inorder_tag) {
            if (ptr_->left != nullptr) {
                ptr_ = ptr_->left;
                while (ptr_->right != nullptr) {
                    ptr_ = ptr_->right;
                }
                return *this;
            }
            Node* par = ptr_->parent;
            while (par != ptr_ && ptr_ == par->left) {
                ptr_ = par;
                par = par->parent;
            }
            ptr_ = par;
            return *this;
        }

        base_iterator operator--(int) {
            auto copy = *this;
            --ptr_;
            return copy;
        }

    private:
        value_type* ptr_;

        base_iterator(value_type* ptr) : ptr_(ptr) {}
    };

public:
    template <typename OrderType = inorder_tag>
    using iterator = base_iterator<OrderType>;

    template <typename OrderType = inorder_tag>
    using const_iterator = base_iterator<OrderType>;

    template <typename OrderType = inorder_tag>
    using reverse_iterator = std::reverse_iterator<iterator<OrderType>>;

    template <typename OrderType = inorder_tag>
    using const_reverse_iterator = std::reverse_iterator<const_iterator<OrderType>>;

    using value_type = T;
    using reference = T&;
    using const_reference = const T&;

    template <typename OrderType = inorder_tag>
    using difference_type = std::iterator_traits<iterator<OrderType>>::difference_type;

    using size_type = size_t;
    using node_type = Node;

    using key_type = T;
    using key_compare = Compare;
    using value_compare = Compare;

    using AllocTraits = std::allocator_traits<typename std::allocator_traits<Alloc>::template rebind_alloc<node_type>>;
    using allocator_type = Alloc;

    node_type* beg_(inorder_tag) const { return fake_node_->right; }

    node_type* beg_(preorder_tag) const { return fake_node_->left; }

    node_type* beg_(postorder_tag) const { return fake_node_->right; }

    template <typename OrderType = inorder_tag>
    const_iterator<OrderType> begin() const { return iterator<OrderType>(beg_(OrderType{})); }

    template <typename OrderType = inorder_tag>
    const_iterator<OrderType> end() const { return iterator<OrderType>(fake_node_); }

    template <typename OrderType = inorder_tag>
    const_iterator<OrderType> cbegin() const { return iterator<OrderType>(begin<OrderType>()); }

    template <typename OrderType = inorder_tag>
    const_iterator<OrderType> cend() const { return iterator<OrderType>(end<OrderType>()); }

    template <typename OrderType = inorder_tag>
    reverse_iterator<OrderType> rbegin() const { return reverse_iterator<OrderType>(end<OrderType>()); }

    template <typename OrderType = inorder_tag>
    reverse_iterator<OrderType> rend() const { return reverse_iterator<OrderType>(begin<OrderType>()); }

    template <typename OrderType = inorder_tag>
    const_reverse_iterator<OrderType> crbegin() const { return reverse_iterator<OrderType>(end<OrderType>()); }

    template <typename OrderType = inorder_tag>
    const_reverse_iterator<OrderType> crend() const {return reverse_iterator<OrderType>(begin<OrderType>()); }

    BinarySearchTree() : fake_node_(&base_node_), size_(0), comp_(), alloc_() {
        fake_node_->parent = static_cast<node_type*>(fake_node_);
        fake_node_->right = static_cast<node_type*>(fake_node_);
    }

    BinarySearchTree(const_reference element) : fake_node_(&base_node_), size_(1), comp_(), alloc_() {
        fake_node_->left = alloc_.allocate(1);
        AllocTraits::construct(alloc_, fake_node_->left, element);
        fake_node_->left->parent = static_cast<node_type*>(fake_node_);
        fake_node_->right = fake_node_->left;
        fake_node_->parent = static_cast<Node*>(fake_node_);
    }

    ~BinarySearchTree() {
        clear();
    }

    size_type size() const { return size_; }

    size_type max_size() const { std::numeric_limits<size_type>::max(); }

    bool empty() const { return (begin() == end()) ? true : false; }

    template <typename OrderType = inorder_tag>
    void insert(OrderType first, OrderType second) {
        while (first != second) {
            insert(*first);
            ++first;
        }
    }

    void insert(const std::initializer_list<value_type>& il) {
        insert(il.begin(), il.end());
    }

    BinarySearchTree(const std::initializer_list<value_type>& il) 
    : fake_node_(&base_node_), size_(0), comp_(), alloc_() {
        insert(il);
    }

    BinarySearchTree(const std::initializer_list<value_type>& il, key_compare comp) 
    : fake_node_(&base_node_), size_(0), comp_(comp), alloc_() {
        insert(il);
    }

    template <typename OrderType = inorder_tag>
    BinarySearchTree(base_iterator<OrderType>& first, base_iterator<OrderType>& second) 
    : fake_node_(&base_node_), size_(0), comp_(), alloc_() {
        insert(first, second);
    }

    template <typename OrderType = inorder_tag>
    BinarySearchTree(base_iterator<OrderType>& first, base_iterator<OrderType>& second, key_compare comp) 
    : fake_node_(&base_node_), size_(0), comp_(comp), alloc_() {
        insert(first, second);
    }

    BinarySearchTree(const BinarySearchTree& other) 
    : base_node_(), fake_node_(&base_node_), size_(0), comp_(other.comp_), alloc_(other.alloc_) {
        fake_node_->parent = static_cast<node_type*>(fake_node_);
        fake_node_->right = static_cast<node_type*>(fake_node_);
        for (auto el = other.begin<preorder_tag>(); el != other.end<preorder_tag>(); ++el) {
            insert(*el);
        }
    }

    BinarySearchTree& operator=(std::initializer_list<value_type> il) {
        clear();
        insert(il);
        return *this;
    }

    BinarySearchTree& operator=(const BinarySearchTree& other) {
        if (*this == other) {
            return *this;
        }
        clear();
        alloc_ = other.alloc_;
        comp_ = other.comp_;
        for (auto el = other.begin<preorder_tag>(); el != other.end<preorder_tag>(); ++el) {
            insert(*el);
        }
        return *this;
    }

    template <typename OrderType = inorder_tag>
    iterator<OrderType> find(const_reference key) const {
        node_type* now = static_cast<node_type*>(fake_node_->left);
        while (now != nullptr) {
            if ((now != nullptr) && (now->data_ == key)) {
                return iterator<OrderType>{now};
            }
            if (comp_(key, now->data_)) {
                now = now->left;
            } else if (comp_(now->data_, key)) {
                now = now->right;
            }
        }
        return end();
    }

    bool insert_spec(inorder_tag, Node* elem) const { return false; }

    bool insert_spec(preorder_tag, Node* elem) const { return false; }

    bool insert_spec(postorder_tag, Node* elem) const { return ((elem != nullptr)) ? false : true; }

    template <typename OrderType = inorder_tag>
    std::pair<iterator<OrderType>, bool> insert(const_reference key) {
        node_type* val = alloc_.allocate(1);
        AllocTraits::construct(alloc_, val, key);
        if (fake_node_->left == nullptr) {
            fake_node_->left = val;
            fake_node_->left->parent = static_cast<node_type*>(fake_node_);
            fake_node_->right = val;
            ++size_;
            return std::pair(iterator<OrderType>{val}, true);
        }
        node_type* now = static_cast<node_type*>(fake_node_->left);
        bool flag = true;
        while (now != nullptr) {
            if (comp_(now->data_, val->data_)) {
                flag = insert_spec(OrderType{}, now->left);
                if (now->right != nullptr) {
                    now = now->right;
                } else {
                    now->right = val;
                    val->parent = now;
                    break;
                }
            } else if (comp_(val->data_, now->data_)) {
                if (now->left != nullptr) {
                    now = now->left;
                } else {
                    now->left = val;
                    val->parent = now;
                    break;
                }
            } else {
                return std::pair(iterator<OrderType>{now}, false);
            }
        }
        if (flag) {
            fake_node_->right = val;
        }
        ++size_;
        return std::pair(iterator<OrderType>{val}, true);
    }

    template <typename OrderType = inorder_tag>
    iterator<OrderType> erase(iterator<OrderType> val) { 
        node_type* elem = static_cast<node_type*>(val.ptr_);
        node_type* par = static_cast<node_type*>(elem->parent);
        if (elem->left == nullptr && elem->right == nullptr) {
            if (par == fake_node_) {
                fake_node_->left = nullptr;
                fake_node_->right = static_cast<node_type*>(fake_node_);
            } else {
                
                if (par->left == elem) {
                    par->left = nullptr;
                    if (elem == fake_node_->right) {
                        fake_node_->right = par;
                    }
                } else {
                    if (fake_node_->right == elem) {
                        fake_node_->right = par;
                    }
                    par->right = nullptr;
                }
            
            }
            ++val;
            AllocTraits::destroy(alloc_, elem);
            alloc_.deallocate(elem, 1);
            return val;
        } else if (elem->left == nullptr || elem->right == nullptr) {
            if (elem->left == nullptr) {
                if (par == fake_node_) {
                    elem->right->parent = static_cast<node_type*>(fake_node_);
                    fake_node_->left = elem->right;
                    Node* sub = elem->right;
                    while (sub->left != nullptr) {
                        sub = sub->left;
                    }
                    fake_node_->right = sub;
                    
                } else {
                    if (par->left == elem) {
                        par->left = elem->right;
                    } else {
                        par->right = elem->right;
                    }
                    elem->right->parent = par;
                    if (elem == fake_node_->right) {
                        node_type* sub = elem->right;
                        while (sub->left != nullptr) {
                            sub = sub->left;
                        }
                        fake_node_->right = sub;
                    }
                }
            } else {
                if (par == fake_node_) {
                    elem->left->parent = static_cast<node_type*>(fake_node_);
                    fake_node_->left = elem->left;
                } else {
                    if (par->left == elem) {
                        par->left = elem->left;
                    } else {
                        par->right = elem->left;
                    }
                    elem->left->parent = par;
                }
            }
            ++val;
            AllocTraits::destroy(alloc_, elem);
            alloc_.deallocate(elem, 1);
            return val;
        } else {
            auto it = val;
            ++val;
            node_type* next = static_cast<node_type*>(val.ptr_);
            elem->data_ = next->data_;
            if (next->parent->left == next) {
                next->parent->left = next->right;
                if (next->right != nullptr) {
                    next->right->parent = next->parent;
                }
            } else {
                next->parent->right = next->right;
                if (next->right != nullptr) {
                    next->right->parent = next->parent;
                }
            }
            if (par == fake_node_) {
                fake_node_->left = elem;
            }
            AllocTraits::destroy(alloc_, next);
            alloc_.deallocate(next, 1);
            return it;
        }
        --size_;
    }

    size_type erase(const_reference key) {
        auto it = find(key);
        if (it != end()) {
            erase(it);
            return 1;
        }
        return 0;
    } 

    template <typename OrderType = inorder_tag>
    iterator<OrderType> erase(iterator<OrderType> st, iterator<OrderType> fn) {
        while (st != fn) {
            st = erase<OrderType>(st);
        }
        return fn;
    }

    void clear() { erase(begin(), end()); }

    bool contains(const_reference key) const {
        return (find(key) != end());
    }

    size_type count(const_reference key) const {
        return (contains(key)) ? 1 : 0;
    }

    template <typename OrderType = inorder_tag>
    iterator<OrderType> lower_bound(const_reference key) const {
        if (fake_node_->left == nullptr) {
            return end();
        }
        node_type* now = static_cast<node_type*>(fake_node_->left);
        node_type* best = nullptr;
        while (now != nullptr) {
            if ((now->data_ >= key) && (best == nullptr || best->data_ >= now->data_)) {
                best = now;
            }
            if (comp_(key, now->data_)) {
                now = now->left;
                
            } else if (comp_(now->data_, key)) {
                now = now->right;
            } else {
                return {now};
            }
        }
        if (best == nullptr) {
            return end();
        }
        return {best};
    }

    
    template <typename OrderType = inorder_tag>
    iterator<OrderType> upper_bound(const_reference key) const {
        if (fake_node_->left == nullptr) {
            return end();
        }
        node_type* now = static_cast<node_type*>(fake_node_->left);
        node_type* best = nullptr;
        while (now != nullptr) {
            if ((now->data_ > key) && (best == nullptr || best->data_ >= now->data_)) {
                best = now;
            }
            if (comp_(key, now->data_)) {
                now = now->left;
                
            } else if (comp_(now->data_, key)) {
                now = now->right;
            } else {
                now = now->right;
            }
        }
        if (best == nullptr) {
            return end();
        }
        return {best};
    }

    template <typename OrderType = inorder_tag>
    std::pair<iterator<OrderType>, iterator<OrderType>> equal_range(const_reference key) const {
        return std::make_pair(lower_bound(key), upper_bound(key));
    }

    key_compare key_comp() const { return comp_; }

    value_compare value_comp() const { return comp_; }

    allocator_type get_allocator() const {return alloc_; }

};

template <typename T, typename Compare = std::less<T>, typename Alloc = std::allocator<T>>
bool operator==(const BinarySearchTree<T, Compare, Alloc>& first, 
                const BinarySearchTree<T, Compare, Alloc>& second) {
    return (std::equal(first.begin(), first.end(), second.begin(), second.end())) ? true : false;
}

template <typename T, typename Compare = std::less<T>, typename Alloc = std::allocator<T>>
bool operator!=(const BinarySearchTree<T, Compare, Alloc>& first, 
                const BinarySearchTree<T, Compare, Alloc>& second) { 
    return !(first == second); 
}
