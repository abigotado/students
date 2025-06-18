#pragma once

#include <vector>
#include <optional>
#include <memory>
#include <stdexcept>
#include <functional>
#include <iterator>
#include <cstddef>
#include <utility>

namespace university
{

    /**
     * @class HashTable
     * @brief Хеш-таблица, отображающая ключи в значения.
     *
     * Эта реализация использует линейное пробирование для разрешения коллизий.
     * Таблица автоматически изменяет размер, когда коэффициент загрузки превышает пороговое значение.
     * Выбор итератора - прямой итератор, потому что хеш-таблица является неупорядоченным контейнером.
     * Прямой итератор предоставляет необходимую функциональность для обхода всех элементов
     * без подразумевания какого-либо порядка, что идеально подходит для характеристик контейнера.
     *
     * @tparam Key Тип ключей.
     * @tparam Value Тип значений.
     */
    template <typename Key, typename Value>
    class HashTable
    {
    private:
        struct Entry
        {
            Key key;
            Value value;
            bool occupied = false;
            bool deleted = false;

            Entry() = default;
            Entry(Entry &&other) noexcept = default;
            Entry &operator=(Entry &&other) noexcept = default;

            // Удалённые конструктор копирования и оператор присваивания
            Entry(const Entry &) = delete;
            Entry &operator=(const Entry &) = delete;
        };

    public:
        class Iterator
        {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = std::pair<const Key, Value>;

            Iterator(typename std::vector<Entry>::iterator ptr, typename std::vector<Entry>::iterator end) : current_(ptr), end_(end) { advanceToNextValid(); }

            std::pair<const Key &, Value &> operator*() const { return {current_->key, current_->value}; }
            Iterator &operator++()
            {
                ++current_;
                advanceToNextValid();
                return *this;
            }
            bool operator!=(const Iterator &other) const { return current_ != other.current_; }
            bool operator==(const Iterator &other) const { return current_ == other.current_; }

        private:
            void advanceToNextValid()
            {
                while (current_ != end_ && (!current_->occupied || current_->deleted))
                {
                    ++current_;
                }
            }
            typename std::vector<Entry>::iterator current_;
            typename std::vector<Entry>::iterator end_;
        };

        class ConstIterator
        {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = std::pair<const Key, const Value>;

            ConstIterator(typename std::vector<Entry>::const_iterator ptr, typename std::vector<Entry>::const_iterator end) : current_(ptr), end_(end) { advanceToNextValid(); }

            std::pair<const Key &, const Value &> operator*() const { return {current_->key, current_->value}; }
            ConstIterator &operator++()
            {
                ++current_;
                advanceToNextValid();
                return *this;
            }
            bool operator!=(const ConstIterator &other) const { return current_ != other.current_; }
            bool operator==(const ConstIterator &other) const { return current_ == other.current_; }

        private:
            void advanceToNextValid()
            {
                while (current_ != end_ && (!current_->occupied || current_->deleted))
                {
                    ++current_;
                }
            }
            typename std::vector<Entry>::const_iterator current_;
            typename std::vector<Entry>::const_iterator end_;
        };

        /**
         * @brief Предоставляет итератор к началу контейнера.
         * @return Итератор к первому элементу.
         */
        Iterator begin() { return Iterator(table_.begin(), table_.end()); }

        /**
         * @brief Предоставляет итератор к концу контейнера.
         * @return Итератор к элементу, следующему за последним.
         */
        Iterator end() { return Iterator(table_.end(), table_.end()); }

        /**
         * @brief Предоставляет итератор к началу контейнера (const версия).
         * @return Итератор к первому элементу.
         */
        ConstIterator begin() const { return ConstIterator(table_.begin(), table_.end()); }

        /**
         * @brief Предоставляет итератор к концу контейнера (const версия).
         * @return Итератор к элементу, следующему за последним.
         */
        ConstIterator end() const { return ConstIterator(table_.end(), table_.end()); }

        /**
         * @brief Предоставляет итератор к началу контейнера (const версия).
         * @return Итератор к первому элементу.
         */
        ConstIterator cbegin() const { return ConstIterator(table_.cbegin(), table_.cend()); }

        /**
         * @brief Предоставляет итератор к концу контейнера (const версия).
         * @return Итератор к элементу, следующему за последним.
         */
        ConstIterator cend() const { return ConstIterator(table_.cend(), table_.cend()); }

    private:
        std::vector<Entry> table_;
        size_t size_ = 0;
        float maxLoadFactor_ = 0.7f;

        /**
         * @brief Вычисляет хеш ключа.
         * @param key Ключ для хеширования.
         * @return Значение хеша.
         */
        size_t hash(const Key &key, size_t tableSize) const
        {
            if (tableSize == 0)
            {
                return 0;
            }
            return std::hash<Key>{}(key) % tableSize;
        }

        /**
         * @brief Изменяет размер хеш-таблицы до удвоенного текущего размера или до минимального размера, если она пуста.
         */
        void rehash()
        {
            size_t newSize = table_.empty() ? 16 : table_.size() * 2;
            std::vector<Entry> newTable(newSize);
            size_ = 0;
            for (auto &entry : table_)
            {
                if (entry.occupied && !entry.deleted)
                {
                    size_t index = hash(entry.key, newSize);
                    while (newTable[index].occupied)
                    {
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
         * @brief Конструирует новый объект HashTable.
         * @param initialCapacity Начальная ёмкость хеш-таблицы.
         */
        explicit HashTable(size_t initialCapacity = 16)
        {
            table_.resize(initialCapacity);
        }

        /**
         * @brief Вставляет пару ключ-значение в хеш-таблицу.
         * @param key Ключ для вставки.
         * @param value Значение, связанное с ключом.
         * @return True, если вставка была успешной, false, если ключ уже существует.
         * @throw std::runtime_error если хеш-таблица заполнена и не может быть изменена.
         */
        bool insert(const Key &key, Value value)
        {
            if (table_.empty() || size_ >= table_.size() * maxLoadFactor_)
            {
                rehash();
            }

            size_t firstDeleted = SIZE_MAX;

            // Линейное пробирование
            for (size_t i = 0; i < table_.size(); ++i)
            {
                size_t index = (hash(key, table_.size()) + i) % table_.size();

                if (table_[index].deleted && firstDeleted == SIZE_MAX)
                {
                    firstDeleted = index;
                }

                if (table_[index].occupied && table_[index].key == key && !table_[index].deleted)
                {
                    return false; // Ключ уже существует
                }

                if (!table_[index].occupied && !table_[index].deleted)
                {
                    size_t insert_pos = (firstDeleted != SIZE_MAX) ? firstDeleted : index;
                    table_[insert_pos].key = key;
                    table_[insert_pos].value = std::move(value);
                    table_[insert_pos].occupied = true;
                    table_[insert_pos].deleted = false;
                    size_++;

                    // Проверяем коэффициент загрузки
                    if (static_cast<float>(size_) / table_.size() > maxLoadFactor_)
                    {
                        rehash();
                    }
                    return true;
                }
            }

            // Если нашли удалённую позицию, используем её
            if (firstDeleted != SIZE_MAX)
            {
                table_[firstDeleted].key = key;
                table_[firstDeleted].value = std::move(value);
                table_[firstDeleted].occupied = true;
                table_[firstDeleted].deleted = false;
                size_++;
                return true;
            }

            throw std::runtime_error("Хеш-таблица заполнена.");
        }

        /**
         * @brief Находит значение, связанное с данным ключом.
         * @param key Ключ для поиска.
         * @return Optional, содержащий ссылку на значение, если ключ найден, иначе пустой optional.
         */
        std::optional<std::reference_wrapper<Value>> find(const Key &key)
        {
            if (empty())
                return std::nullopt;
            size_t index = hash(key, table_.size());
            size_t startIndex = index;
            do
            {
                if (!table_[index].occupied && !table_[index].deleted)
                {
                    return std::nullopt; // Ключ не найден
                }
                if (table_[index].key == key && !table_[index].deleted)
                {
                    return table_[index].value;
                }
                index = (index + 1) % table_.size();
            } while (index != startIndex);
            return std::nullopt;
        }

        /**
         * @brief Находит значение, связанное с данным ключом (const версия).
         * @param key Ключ для поиска.
         * @return Optional, содержащий const ссылку на значение, если ключ найден, иначе пустой optional.
         */
        std::optional<std::reference_wrapper<const Value>> find(const Key &key) const
        {
            if (empty())
                return std::nullopt;
            size_t index = hash(key, table_.size());
            size_t startIndex = index;
            do
            {
                if (!table_[index].occupied && !table_[index].deleted)
                {
                    return std::nullopt; // Ключ не найден
                }
                if (table_[index].key == key && !table_[index].deleted)
                {
                    return table_[index].value;
                }
                index = (index + 1) % table_.size();
            } while (index != startIndex);
            return std::nullopt;
        }

        /**
         * @brief Удаляет пару ключ-значение из хеш-таблицы.
         * @param key Ключ для удаления.
         * @return True, если удаление было успешным, false, если ключ не найден.
         */
        bool remove(const Key &key)
        {
            if (empty())
                return false;
            size_t index = hash(key, table_.size());
            size_t startIndex = index;
            do
            {
                if (!table_[index].occupied)
                {
                    return false; // Ключ не найден
                }
                if (table_[index].key == key && !table_[index].deleted)
                {
                    table_[index].deleted = true;
                    size_--;
                    return true;
                }
                index = (index + 1) % table_.size();
            } while (index != startIndex);
            return false;
        }

        /**
         * @brief Получает количество элементов в хеш-таблице.
         * @return Количество элементов.
         */
        [[nodiscard]] size_t size() const
        {
            return size_;
        }

        /**
         * @brief Проверяет, пуста ли хеш-таблица.
         * @return True, если хеш-таблица пуста, false в противном случае.
         */
        [[nodiscard]] bool empty() const
        {
            return size_ == 0;
        }
    };

} // namespace university
