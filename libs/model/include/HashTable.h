#pragma once

#include <vector>
#include <optional>
#include <memory>
#include <stdexcept>
#include <functional>
#include <iterator>
#include <cstddef>
#include <utility>

namespace university {

/**
 * @class HashTable
 * @brief A hash table container mapping keys to values.
 *
 * This implementation uses linear probing for collision resolution.
 * The table automatically resizes when the load factor exceeds a threshold.
 * The choice of iterator is a forward iterator because a hash table is an
 * unordered container. A forward iterator provides the necessary functionality
 * to traverse all elements without implying any order, which is a perfect
 * match for the container's characteristics.
 *
 * @tparam Key The type of the keys.
 * @tparam Value The type of the values.
 */
template<typename Key, typename Value>
class HashTable {
private:
    struct Entry {
        Key key;
        Value value;
        bool occupied = false;
        bool deleted = false;
    };

public:
    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<const Key, Value>;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        Iterator(typename std::vector<Entry>::iterator ptr, typename std::vector<Entry>::iterator end)
            : current_(ptr), end_(end) {
            advanceToNextValid();
        }

        reference operator*() const {
            return *reinterpret_cast<pointer>(&*current_);
        }

        pointer operator->() const {
            return reinterpret_cast<pointer>(&*current_);
        }

        Iterator& operator++() {
            ++current_;
            advanceToNextValid();
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const Iterator& a, const Iterator& b) {
            return a.current_ == b.current_;
        }

        friend bool operator!=(const Iterator& a, const Iterator& b) {
            return a.current_ != b.current_;
        }

    private:
        void advanceToNextValid() {
            while (current_ != end_ && (!current_->occupied || current_->deleted)) {
                ++current_;
            }
        }

        typename std::vector<Entry>::iterator current_;
        typename std::vector<Entry>::iterator end_;
    };

    /**
     * @brief Provides an iterator to the beginning of the container.
     * @return An iterator to the first element.
     */
    Iterator begin() {
        return Iterator(table_.begin(), table_.end());
    }

    /**
     * @brief Provides an iterator to the end of the container.
     * @return An iterator to the element following the last element.
     */
    Iterator end() {
        return Iterator(table_.end(), table_.end());
    }

private:
    std::vector<Entry> table_;
    size_t size_ = 0;
    float maxLoadFactor_ = 0.7f;

    /**
     * @brief Computes the hash of a key.
     * @param key The key to hash.
     * @return The hash value.
     */
    size_t hash(const Key& key) const {
        if (table_.empty()) {
            return 0;
        }
        return std::hash<Key>{}(key) % table_.size();
    }

    /**
     * @brief Resizes the hash table to twice its current size or to a minimum size if it's empty.
     */
    void rehash() {
        size_t newSize = table_.empty() ? 16 : table_.size() * 2;
        std::vector<Entry> oldTable = std::move(table_);
        table_.assign(newSize, Entry{});
        size_ = 0;
        for (auto& entry : oldTable) {
            if (entry.occupied && !entry.deleted) {
                // Use move on the value
                insert(entry.key, std::move(entry.value));
            }
        }
    }

public:
    /**
     * @brief Constructs a new HashTable object.
     * @param initialCapacity The initial capacity of the hash table.
     */
    explicit HashTable(size_t initialCapacity = 16) {
        table_.resize(initialCapacity);
    }

    /**
     * @brief Inserts a key-value pair into the hash table.
     * @param key The key to insert.
     * @param value The value to associate with the key.
     * @return True if the insertion was successful, false if the key already exists.
     * @throw std::runtime_error if the hash table is full and cannot be resized.
     */
    bool insert(const Key& key, Value value) {
        if (table_.empty() || size_ >= table_.size() * maxLoadFactor_) {
            rehash();
        }

        size_t index = hash(key);
        size_t startIndex = index;

        do {
            if (!table_[index].occupied || table_[index].deleted) {
                table_[index] = {key, std::move(value), true, false};
                size_++;
                return true;
            }
            if (table_[index].key == key) {
                return false; // Key already exists
            }
            index = (index + 1) % table_.size();
        } while (index != startIndex);

        throw std::runtime_error("Hash table is full.");
    }

    /**
     * @brief Finds the value associated with a given key.
     * @param key The key to find.
     * @return An optional containing a reference wrapper to the value if the key is found, otherwise an empty optional.
     */
    std::optional<std::reference_wrapper<Value>> find(const Key& key) {
        if (empty()) return std::nullopt;
        size_t index = hash(key);
        size_t startIndex = index;
        do {
            if (!table_[index].occupied && !table_[index].deleted) {
                return std::nullopt; // Key not found
            }
            if (table_[index].key == key && !table_[index].deleted) {
                return table_[index].value;
            }
            index = (index + 1) % table_.size();
        } while (index != startIndex);
        return std::nullopt;
    }

    /**
     * @brief Finds the value associated with a given key (const version).
     * @param key The key to find.
     * @return An optional containing a const reference wrapper to the value if the key is found, otherwise an empty optional.
     */
    std::optional<std::reference_wrapper<const Value>> find(const Key& key) const {
        if (empty()) return std::nullopt;
        size_t index = hash(key);
        size_t startIndex = index;
        do {
            if (!table_[index].occupied && !table_[index].deleted) {
                return std::nullopt; // Key not found
            }
            if (table_[index].key == key && !table_[index].deleted) {
                return table_[index].value;
            }
            index = (index + 1) % table_.size();
        } while (index != startIndex);
        return std::nullopt;
    }

    /**
     * @brief Removes a key-value pair from the hash table.
     * @param key The key to remove.
     * @return True if the removal was successful, false if the key was not found.
     */
    bool remove(const Key& key) {
        if (empty()) return false;
        size_t index = hash(key);
        size_t startIndex = index;
        do {
            if (!table_[index].occupied) {
                return false; // Key not found
            }
            if (table_[index].key == key && !table_[index].deleted) {
                table_[index].deleted = true;
                size_--;
                return true;
            }
            index = (index + 1) % table_.size();
        } while (index != startIndex);
        return false;
    }

    /**
     * @brief Gets the number of elements in the hash table.
     * @return The number of elements.
     */
    [[nodiscard]] size_t size() const {
        return size_;
    }

    /**
     * @brief Checks if the hash table is empty.
     * @return True if the hash table is empty, false otherwise.
     */
    [[nodiscard]] bool empty() const {
        return size_ == 0;
    }
};

} // namespace university

 