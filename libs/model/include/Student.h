#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <memory>

namespace university {

/**
 * @enum StudentCategory
 * @brief Определяет категорию студента.
 */
enum class StudentCategory {
    JUNIOR,
    SENIOR,
    GRADUATE
};

/**
 * @class Student
 * @brief Абстрактный базовый класс, представляющий студента.
 *
 * Этот класс предоставляет общий интерфейс для различных типов студентов
 * в университете. Он хранит общую информацию, такую как имя, индекс группы
 * и номер кафедры.
 */
class Student {
public:
    /**
     * @brief Конструирует новый объект Student.
     * @param name Полное имя и инициалы студента.
     * @param groupIndex Индекс группы студента.
     * @param departmentNumber Номер кафедры студента.
     * @throw std::invalid_argument если имя пустое или номер кафедры отрицательный.
     */
    Student(const std::string& name, const std::string& groupIndex, int departmentNumber);

    /**
     * @brief Виртуальный деструктор.
     */
    virtual ~Student() = default;

    /**
     * @brief Получает категорию студента.
     * @return Категория студента.
     */
    [[nodiscard]] virtual StudentCategory getCategory() const = 0;

    /**
     * @brief Выводит информацию о студенте в заданный поток вывода.
     * @param os Поток вывода для записи.
     */
    virtual void printInfo(std::ostream& os) const;

    /**
     * @brief Изменяет индекс группы студента.
     * @param newGroupIndex Новый индекс группы.
     */
    void setGroupIndex(const std::string& newGroupIndex);

    /**
     * @brief Получает имя студента.
     * @return Имя студента.
     */
    [[nodiscard]] const std::string& getName() const { return name_; }

    /**
     * @brief Получает индекс группы студента.
     * @return Индекс группы студента.
     */
    [[nodiscard]] const std::string& getGroupIndex() const { return groupIndex_; }

    /**
     * @brief Получает номер кафедры студента.
     * @return Номер кафедры студента.
     */
    [[nodiscard]] int getDepartmentNumber() const { return departmentNumber_; }

protected:
    std::string name_;
    std::string groupIndex_;
    int departmentNumber_;
};

} // namespace university 