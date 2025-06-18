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

        Entry() = default;
        Entry(Entry&& other) noexcept = default;
        Entry& operator=(Entry&& other) noexcept = default;

        // Deleted copy constructor and assignment operator
        Entry(const Entry&) = delete;
        Entry& operator=(const Entry&) = delete;
    };

public:
    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<const Key, Value>;
        
        Iterator(typename std::vector<Entry>::iterator ptr, typename std::vector<Entry>::iterator end) : current_(ptr), end_(end) { advanceToNextValid(); }
        
        std::pair<const Key&, Value&> operator*() const { return {current_->key, current_->value}; }
        Iterator& operator++() { ++current_; advanceToNextValid(); return *this; }
        bool operator!=(const Iterator& other) const { return current_ != other.current_; }
        bool operator==(const Iterator& other) const { return current_ == other.current_; }

    private:
        void advanceToNextValid() { while (current_ != end_ && (!current_->occupied || current_->deleted)) { ++current_; } }
        typename std::vector<Entry>::iterator current_;
        typename std::vector<Entry>::iterator end_;
    };

    class ConstIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<const Key, const Value>;

        ConstIterator(typename std::vector<Entry>::const_iterator ptr, typename std::vector<Entry>::const_iterator end) : current_(ptr), end_(end) { advanceToNextValid(); }

        std::pair<const Key&, const Value&> operator*() const { return {current_->key, current_->value}; }
        ConstIterator& operator++() { ++current_; advanceToNextValid(); return *this; }
        bool operator!=(const ConstIterator& other) const { return current_ != other.current_; }
        bool operator==(const ConstIterator& other) const { return current_ == other.current_; }

    private:
        void advanceToNextValid() { while (current_ != end_ && (!current_->occupied || current_->deleted)) { ++current_; } }
        typename std::vector<Entry>::const_iterator current_;
        typename std::vector<Entry>::const_iterator end_;
    };

    /**
     * @brief Provides an iterator to the beginning of the container.
     * @return An iterator to the first element.
     */
    Iterator begin() { return Iterator(table_.begin(), table_.end()); }

    /**
     * @brief Provides an iterator to the end of the container.
     * @return An iterator to the element following the last element.
     */
    Iterator end() { return Iterator(table_.end(), table_.end()); }

    /**
     * @brief Provides an iterator to the beginning of the container (const version).
     * @return An iterator to the first element.
     */
    ConstIterator begin() const { return ConstIterator(table_.begin(), table_.end()); }

    /**
     * @brief Provides an iterator to the end of the container (const version).
     * @return An iterator to the element following the last element.
     */
    ConstIterator end() const { return ConstIterator(table_.end(), table_.end()); }

    /**
     * @brief Provides an iterator to the beginning of the container (const version).
     * @return An iterator to the first element.
     */
    ConstIterator cbegin() const { return ConstIterator(table_.cbegin(), table_.cend()); }

    /**
     * @brief Provides an iterator to the end of the container (const version).
     * @return An iterator to the element following the last element.
     */
    ConstIterator cend() const { return ConstIterator(table_.cend(), table_.cend()); }

private:
    std::vector<Entry> table_;
    size_t size_ = 0;
    float maxLoadFactor_ = 0.7f;

    /**
     * @brief Computes the hash of a key.
     * @param key The key to hash.
     * @return The hash value.
     */
    size_t hash(const Key& key, size_t tableSize) const {
        if (tableSize == 0) {
            return 0;
        }
        return std::hash<Key>{}(key) % tableSize;
    }

    /**
     * @brief Resizes the hash table to twice its current size or to a minimum size if it's empty.
     */
    void rehash() {
        size_t newSize = table_.empty() ? 16 : table_.size() * 2;
        std::vector<Entry> newTable(newSize);
        size_ = 0;
        for (auto& entry : table_) {
            if (entry.occupied && !entry.deleted) {
                size_t index = hash(entry.key, newSize);
                while (newTable[index].occupied) {
                    index = (index + 1) % newSize;
                }
                newTable[index].key = entry.key;
                newTable[index].value = std::move(entry.value);
                newTable[index].occupied = true;
                size_++;
            }
        }
        table_ = std::move(newTable);
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

        size_t index = hash(key, table_.size());
        size_t startIndex = index;

        size_t firstDeleted = -1;

        do {
            if (table_[index].deleted && firstDeleted == (size_t)-1) {
                firstDeleted = index;
            }

            if (!table_[index].occupied) {
                size_t insert_pos = (firstDeleted != (size_t)-1) ? firstDeleted : index;
                table_[insert_pos].key = key;
                table_[insert_pos].value = std::move(value);
                table_[insert_pos].occupied = true;
                table_[insert_pos].deleted = false;
                size_++;
                return true;
            }
            if (table_[index].key == key && !table_[index].deleted) {
                return false; // Key already exists
            }
            index = (index + 1) % table_.size();
        } while (index != startIndex);
        
        if (firstDeleted != (size_t)-1) {
             table_[firstDeleted].key = key;
             table_[firstDeleted].value = std::move(value);
             table_[firstDeleted].occupied = true;
             table_[firstDeleted].deleted = false;
             size_++;
             return true;
        }

        throw std::runtime_error("Hash table is full.");
    }

    /**
     * @brief Finds the value associated with a given key.
     * @param key The key to find.
     * @return An optional containing a reference wrapper to the value if the key is found, otherwise an empty optional.
     */
    std::optional<std::reference_wrapper<Value>> find(const Key& key) {
        if (empty()) return std::nullopt;
        size_t index = hash(key, table_.size());
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
        size_t index = hash(key, table_.size());
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
        size_t index = hash(key, table_.size());
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

 