#pragma once

#include "Student.h"
#include <string>

namespace university {

/**
 * @struct DiplomaProject
 * @brief Represents the diploma project of a graduate student.
 */
struct DiplomaProject {
    std::string topic;
    std::string place;
    int supervisorGrade;
    int reviewerGrade;
    int stateCommissionGrade;
};

/**
 * @class GraduateStudent
 * @brief Represents a graduate student.
 */
class GraduateStudent : public Student {
public:
    /**
     * @brief Constructs a new GraduateStudent object.
     * @param name The student's full name.
     * @param groupIndex The student's group index.
     * @param departmentNumber The student's department number.
     * @param diplomaProject The student's diploma project details.
     */
    GraduateStudent(const std::string& name, const std::string& groupIndex, int departmentNumber,
                    const DiplomaProject& diplomaProject);

    /**
     * @brief Gets the student's category.
     * @return Always returns StudentCategory::GRADUATE.
     */
    [[nodiscard]] StudentCategory getCategory() const override;

    /**
     * @brief Prints the student's information to the given output stream.
     * @param os The output stream to write to.
     */
    void printInfo(std::ostream& os) const override;

    /**
     * @brief Gets the student's diploma project information.
     * @return A const reference to the diploma project structure.
     */
    [[nodiscard]] const DiplomaProject& getDiplomaProject() const;

    /**
     * @brief Sets the student's diploma project information.
     * @param diplomaProject The new diploma project details.
     */
    void setDiplomaProject(const DiplomaProject& diplomaProject);

private:
    DiplomaProject diplomaProject_;
};

} // namespace university 