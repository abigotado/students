#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <memory>

namespace university {

/**
 * @enum StudentCategory
 * @brief Defines the category of a student.
 */
enum class StudentCategory {
    JUNIOR,
    SENIOR,
    GRADUATE
};

/**
 * @class Student
 * @brief An abstract base class representing a student.
 *
 * This class provides a common interface for different types of students
 * in the university. It stores common information such as name, group index,
 * and department number.
 */
class Student {
public:
    /**
     * @brief Constructs a new Student object.
     * @param name The student's full name and initials.
     * @param groupIndex The index of the student's group.
     * @param departmentNumber The number of the student's department.
     * @throw std::invalid_argument if name is empty or departmentNumber is negative.
     */
    Student(const std::string& name, const std::string& groupIndex, int departmentNumber);

    /**
     * @brief Virtual destructor.
     */
    virtual ~Student() = default;

    /**
     * @brief Gets the student's category.
     * @return The category of the student.
     */
    [[nodiscard]] virtual StudentCategory getCategory() const = 0;

    /**
     * @brief Prints the student's information to the given output stream.
     * @param os The output stream to write to.
     */
    virtual void printInfo(std::ostream& os) const;

    /**
     * @brief Changes the student's group index.
     * @param newGroupIndex The new group index.
     */
    void setGroupIndex(const std::string& newGroupIndex);

protected:
    std::string name_;
    std::string groupIndex_;
    int departmentNumber_;
};

} // namespace university 