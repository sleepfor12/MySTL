#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H

#include <functional>
#include <memory>
#include <initializer_list>

namespace mystl {
    // 前置声明
    template<
        class Key,
        class T,
        class Hash = std::hash<Key>,
        class KeyEqual = std::equal_to<Key>,
        class Allocator = std::allocator<std::pair<const Key, T> > >
    class unordered_map;

    // 非成员函数声明
    template<class Key, class T, class Hash, class KeyEqual, class Allocator>
    bool operator==(const unordered_map<Key, T, Hash, KeyEqual, Allocator> &lhs,
                    const unordered_map<Key, T, Hash, KeyEqual, Allocator> &rhs);

    template<class Key, class T, class Hash, class KeyEqual, class Allocator>
    bool operator!=(const unordered_map<Key, T, Hash, KeyEqual, Allocator> &lhs,
                    const unordered_map<Key, T, Hash, KeyEqual, Allocator> &rhs);

    template<class Key, class T, class Hash, class KeyEqual, class Allocator>
    void swap(unordered_map<Key, T, Hash, KeyEqual, Allocator> &lhs,
              unordered_map<Key, T, Hash, KeyEqual, Allocator> &rhs);

    template<class Key, class T, class Hash, class KeyEqual, class Allocator, class Pred>
    typename unordered_map<Key, T, Hash, KeyEqual, Allocator>::size_type
    erase_if(unordered_map<Key, T, Hash, KeyEqual, Allocator> &c, Pred pred);

    // 主要类模板定义
    template<
        class Key,
        class T,
        class Hash,
        class KeyEqual,
        class Allocator
    >
    class unordered_map {
        // 成员类型
        using key_type = Key;
        using mapped_type = T;
        using value_type = std::pair<const Key, T>;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using hasher = Hash;
        using key_equal = KeyEqual;
        using allocator_type = Allocator;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;

    private:
        struct node_type {
            std::pair<const Key, T> value;
            node_type *next;
            std::size_t hash;

            template<typename... Args>
            node_type(Args &&... args) : value(std::forward<Args>(args)...), next(nullptr), hash(0) {
            }
        };

        node_type **buckets_;
        std::size_t bucket_count_;
        std::size_t element_count_;
        float max_load_factor_;
        hasher hash_;
        key_equal equal_;
        allocator_type alloc_;

    public:
        class iterator {
        private:
            node_type *current_;
            unordered_map *map_;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = unordered_map::value_type;
            using difference_type = unordered_map::difference_type;
            using pointer = unordered_map::pointer;
            using reference = unordered_map::reference;

            iterator() : current_(nullptr), map_(nullptr) {
            }

            iterator(node_type *node, unordered_map *map) : current_(node), map_(map) {
            }

            reference operator*() const { return current_->value; }
            pointer operator->() const { return &current_->value; }

            iterator &operator++();

            iterator operator++(int);

            bool operator==(const iterator &other) const { return current_ == other.current_; }
            bool operator!=(const iterator &other) const { return current_ != other.current_; }
        };

        class const_iterator {
        private:
            node_type *current_;
            unordered_map *map_;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = unordered_map::value_type;
            using difference_type = unordered_map::difference_type;
            using pointer = unordered_map::pointer;
            using reference = unordered_map::reference;

            const_iterator() : current_(nullptr), map_(nullptr) {
            }

            const_iterator(const node_type *node, const unordered_map *map) : current_(node), map_(map) {
            }

            const_iterator(const iterator &it) : current_(it.current_), map_(it.map_) {
            }

            reference operator*() const { return current_->value; }
            pointer operator->() const { return &current_->value; }

            const const_iterator &operator++();

            const_iterator operator++(int);

            bool operator==(const const_iterator &other) const { return current_ == other.current_; }
            bool operator!=(const const_iterator &other) const { return current_ != other.current_; }
        };

        using local_iterator = iterator;
        using const_local_iterator = const_iterator;

        class node_type_handle {
        private:
            node_type* node_;
            allocator_type alloc_;
        };

        // 构造/复制/销毁
        unordered_map() : buckets_(nullptr), bucket_count_(0), element_count_(0), max_load_factor_(1.0f) {
        }

        explicit unordered_map(size_type bucket_count,
                               const Hash &hash = Hash(),
                               const key_equal &equal = key_equal(),
                               const Allocator &alloc = Allocator()) : buckets_(nullptr), bucket_count_(bucket_count),
                                                                       element_count_(0), max_load_factor_(1.0f),
                                                                       hash_(hash), equal_(equal), alloc_(alloc) {
            initialize_buckets();
        }

        unordered_map(size_type bucket_count, const Allocator &alloc);

        unordered_map(size_type bucket_count, const Hash &hash, const Allocator &alloc);

        explicit unordered_map(const Allocator &alloc);

        template<class InputIt>
        unordered_map(InputIt first, InputIt last,
                      size_type bucket_count =  /* 实现定义 */,
                      const Hash &hash = Hash(),
                      const key_equal &equal = key_equal(),
                      const Allocator &alloc = Allocator());

        template<class InputIt>
        unordered_map(InputIt first, InputIt last,
                      size_type bucket_count, const Allocator &alloc);

        template<class InputIt>
        unordered_map(InputIt first, InputIt last,
                      size_type bucket_count, const Hash &hash, const Allocator &alloc);

        unordered_map(const unordered_map &other);

        unordered_map(const unordered_map &other, const Allocator &alloc);

        unordered_map(unordered_map &&other);

        unordered_map(unordered_map &&other, const Allocator &alloc);

        unordered_map(std::initializer_list<value_type> init,
                      size_type bucket_count =  /* 实现定义 */,
                      const Hash &hash = Hash(),
                      const key_equal &equal = key_equal(),
                      const Allocator &alloc = Allocator());

        unordered_map(std::initializer_list<value_type> init,
                      size_type bucket_count, const Allocator &alloc);

        unordered_map(std::initializer_list<value_type> init,
                      size_type bucket_count, const Hash &hash, const Allocator &alloc);

        ~unordered_map() {
            clear();
            if (buckets_) {
                delete[] buckets_;
            }
        }

        unordered_map &operator=(const unordered_map &other);

        unordered_map &operator=(unordered_map &&other) noexcept(/* 见定义 */);

        unordered_map &operator=(std::initializer_list<value_type> init);

        allocator_type get_allocator() const noexcept;

        // 迭代器
        iterator begin() noexcept;

        const_iterator begin() const noexcept;

        const_iterator cbegin() const noexcept;

        iterator end() noexcept;

        const_iterator end() const noexcept;

        const_iterator cend() const noexcept;

        // 容量
        bool empty() const noexcept;

        size_type size() const noexcept;

        size_type max_size() const noexcept;

        // 修改器
        template<class... Args>
        std::pair<iterator, bool> emplace(Args &&... args);

        template<class... Args>
        iterator emplace_hint(const_iterator hint, Args &&... args);

        std::pair<iterator, bool> insert(const value_type &value);

        std::pair<iterator, bool> insert(value_type &&value);

        template<class P>
        std::pair<iterator, bool> insert(P &&value);

        iterator insert(const_iterator hint, const value_type &value);

        iterator insert(const_iterator hint, value_type &&value);

        template<class P>
        iterator insert(const_iterator hint, P &&value);

        template<class InputIt>
        void insert(InputIt first, InputIt last);

        void insert(std::initializer_list<value_type> init);

        template<class M>
        std::pair<iterator, bool> insert_or_assign(const key_type &k, M &&obj);

        template<class M>
        std::pair<iterator, bool> insert_or_assign(key_type &&k, M &&obj);

        template<class M>
        iterator insert_or_assign(const_iterator hint, const key_type &k, M &&obj);

        template<class M>
        iterator insert_or_assign(const_iterator hint, key_type &&k, M &&obj);

        // 擦除
        iterator erase(iterator pos);

        iterator erase(const_iterator pos);

        iterator erase(const_iterator first, const_iterator last);

        size_type erase(const key_type &key);

        void clear() noexcept;

        template<class K>
        size_type erase(K &&x);

        // 交换
        void swap(unordered_map &other) noexcept(/* 见定义 */);

        // C++17 节点处理
        node_type extract(const_iterator position);

        node_type extract(const key_type &x);

        template<class K>
        node_type extract(K &&x);

        insert_return_type insert(node_type &&nh);

        iterator insert(const_iterator hint, node_type &&nh);

        // 查找
        T &at(const key_type &key);

        const T &at(const key_type &key) const;

        T &operator[](const key_type &key);

        T &operator[](key_type &&key);

        size_type count(const key_type &key) const;

        template<class K>
        size_type count(const K &x) const;

        iterator find(const key_type &key);

        const_iterator find(const key_type &key) const;

        template<class K>
        iterator find(const K &x);

        template<class K>
        const_iterator find(const K &x) const;

        bool contains(const key_type &key) const;

        template<class K>
        bool contains(const K &x) const;

        std::pair<iterator, iterator> equal_range(const key_type &key);

        std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const;

        template<class K>
        std::pair<iterator, iterator> equal_range(const K &x);

        template<class K>
        std::pair<const_iterator, const_iterator> equal_range(const K &x) const;

        // 桶接口
        local_iterator begin(size_type n);

        const_local_iterator begin(size_type n) const;

        const_local_iterator cbegin(size_type n) const;

        local_iterator end(size_type n);

        const_local_iterator end(size_type n) const;

        const_local_iterator cend(size_type n) const;

        size_type bucket_count() const noexcept;

        size_type max_bucket_count() const noexcept;

        size_type bucket_size(size_type n) const;

        size_type bucket(const key_type &key) const;

        template<class K>
        size_type bucket(const K &x) const;

        // 哈希策略
        float load_factor() const noexcept;

        float max_load_factor() const noexcept;

        void max_load_factor(float ml);

        void rehash(size_type count);

        void reserve(size_type count);

        // 观察器
        hasher hash_function() const;

        key_equal key_eq() const;
    };

    // 非成员函数
    template<class Key, class T, class Hash, class KeyEqual, class Allocator>
    bool operator==(const unordered_map<Key, T, Hash, KeyEqual, Allocator> &lhs,
                    const unordered_map<Key, T, Hash, KeyEqual, Allocator> &rhs) {
        if (lhs.size() != rhs.size()) { return false; }
        for (const auto &pair: lhs) {
            if (auto it = rhs.find(pair.first); it == rhs.end() || !lhs.key_equal()(it->second, pair.second)) {
                return false;
            }
        }

        return true;
    }

    template<class Key, class T, class Hash, class KeyEqual, class Allocator>
    bool operator!=(const unordered_map<Key, T, Hash, KeyEqual, Allocator> &lhs,
                    const unordered_map<Key, T, Hash, KeyEqual, Allocator> &rhs) {
        return !(lhs == rhs);
    }

    template<class Key, class T, class Hash, class KeyEqual, class Allocator>
    void swap(unordered_map<Key, T, Hash, KeyEqual, Allocator> &lhs,
              unordered_map<Key, T, Hash, KeyEqual, Allocator> &rhs);

    template<class Key, class T, class Hash, class KeyEqual, class Allocator, class Pred>
    typename unordered_map<Key, T, Hash, KeyEqual, Allocator>::size_type
    erase_if(unordered_map<Key, T, Hash, KeyEqual, Allocator> &c, Pred pred);
}

#endif // UNORDERED_MAP_H
