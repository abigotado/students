#pragma once

#include "Student.h"
#include <vector>

namespace university {

/**
 * @class JuniorStudent
 * @brief Represents a junior student.
 */
class JuniorStudent : public Student {
public:
    /**
     * @brief Constructs a new JuniorStudent object.
     * @param name The student's full name.
     * @param groupIndex The student's group index.
     * @param departmentNumber The student's department number.
     * @param sessionGrades A vector of grades for the last session (max 5).
     * @throw std::invalid_argument if sessionGrades contains more than 5 grades.
     */
    JuniorStudent(const std::string& name, const std::string& groupIndex, int departmentNumber, const std::vector<int>& sessionGrades);

    /**
     * @brief Gets the student's category.
     * @return Always returns StudentCategory::JUNIOR.
     */
    [[nodiscard]] StudentCategory getCategory() const override;

    /**
     * @brief Prints the student's information to the given output stream.
     * @param os The output stream to write to.
     */
    void printInfo(std::ostream& os) const override;

    /**
     * @brief Gets the grades for the last session.
     * @return A const reference to the vector of grades.
     */
    [[nodiscard]] const std::vector<int>& getSessionGrades() const;

    /**
     * @brief Sets the grades for the last session.
     * @param grades A vector of grades (max 5).
     * @throw std::invalid_argument if grades contains more than 5 grades.
     */
    void setSessionGrades(const std::vector<int>& grades);

private:
    std::vector<int> sessionGrades_;
};

} // namespace university 