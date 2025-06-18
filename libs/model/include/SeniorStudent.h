#pragma once

#include "Student.h"
#include <vector>
#include <string>

namespace university {

/**
 * @struct ResearchWork
 * @brief Represents the research work of a student.
 */
struct ResearchWork {
    std::string topic;
    std::string place;
    int supervisorGrade;
    int commissionGrade;
};

/**
 * @class SeniorStudent
 * @brief Represents a senior student.
 */
class SeniorStudent : public Student {
public:
    /**
     * @brief Constructs a new SeniorStudent object.
     * @param name The student's full name.
     * @param groupIndex The student's group index.
     * @param departmentNumber The student's department number.
     * @param sessionGrades A vector of grades for the last session (max 4).
     * @param researchWork The student's research work details.
     * @throw std::invalid_argument if sessionGrades contains more than 4 grades.
     */
    SeniorStudent(const std::string& name, const std::string& groupIndex, int departmentNumber,
                  const std::vector<int>& sessionGrades, const ResearchWork& researchWork);

    /**
     * @brief Gets the student's category.
     * @return Always returns StudentCategory::SENIOR.
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
     * @param grades A vector of grades (max 4).
     * @throw std::invalid_argument if grades contains more than 4 grades.
     */
    void setSessionGrades(const std::vector<int>& grades);

    /**
     * @brief Gets the student's research work information.
     * @return A const reference to the research work structure.
     */
    [[nodiscard]] const ResearchWork& getResearchWork() const;

    /**
     * @brief Sets the student's research work information.
     * @param researchWork The new research work details.
     */
    void setResearchWork(const ResearchWork& researchWork);


private:
    std::vector<int> sessionGrades_;
    ResearchWork researchWork_;
};

} // namespace university 