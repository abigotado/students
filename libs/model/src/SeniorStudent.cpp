#include "SeniorStudent.h"
#include <stdexcept>
#include <algorithm>
#include <iterator>

namespace university
{

    namespace
    {
        constexpr size_t MAX_SENIOR_GRADES = 4;
    }

    SeniorStudent::SeniorStudent(const std::string &name, const std::string &groupIndex, int departmentNumber,
                                 const std::vector<int> &sessionGrades, const ResearchWork &researchWork)
        : Student(name, groupIndex, departmentNumber), researchWork_(researchWork)
    {
        setSessionGrades(sessionGrades);
    }

    StudentCategory SeniorStudent::getCategory() const
    {
        return StudentCategory::SENIOR;
    }

    void SeniorStudent::printInfo(std::ostream &os) const
    {
        Student::printInfo(os);
        os << "Category: Senior Student" << std::endl;
        os << "Session Grades: ";
        std::copy(sessionGrades_.begin(), sessionGrades_.end(), std::ostream_iterator<int>(os, " "));
        os << std::endl;
        os << "Research Work Topic: " << researchWork_.topic << std::endl;
        os << "Research Work Place: " << researchWork_.place << std::endl;
        os << "Research Work Grades (Supervisor, Commission): " << researchWork_.supervisorGrade
           << ", " << researchWork_.commissionGrade << std::endl;
    }

    const std::vector<int> &SeniorStudent::getSessionGrades() const
    {
        return sessionGrades_;
    }

    void SeniorStudent::setSessionGrades(const std::vector<int> &grades)
    {
        if (grades.size() > MAX_SENIOR_GRADES)
        {
            throw std::invalid_argument("A senior student can have at most " + std::to_string(MAX_SENIOR_GRADES) + " grades.");
        }
        sessionGrades_ = grades;
    }

    const ResearchWork &SeniorStudent::getResearchWork() const
    {
        return researchWork_;
    }

    void SeniorStudent::setResearchWork(const ResearchWork &researchWork)
    {
        researchWork_ = researchWork;
    }

} // namespace university