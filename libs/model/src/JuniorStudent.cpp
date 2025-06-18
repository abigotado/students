#include "JuniorStudent.h"
#include <stdexcept>
#include <algorithm>
#include <iterator>

namespace university {

namespace {
    constexpr size_t MAX_JUNIOR_GRADES = 5;
}

JuniorStudent::JuniorStudent(const std::string& name, const std::string& groupIndex, int departmentNumber, const std::vector<int>& sessionGrades)
    : Student(name, groupIndex, departmentNumber) {
    setSessionGrades(sessionGrades);
}

StudentCategory JuniorStudent::getCategory() const {
    return StudentCategory::JUNIOR;
}

void JuniorStudent::printInfo(std::ostream& os) const {
    Student::printInfo(os);
    os << "Category: Junior Student" << std::endl;
    os << "Session Grades: ";
    std::copy(sessionGrades_.begin(), sessionGrades_.end(), std::ostream_iterator<int>(os, " "));
    os << std::endl;
}

const std::vector<int>& JuniorStudent::getSessionGrades() const {
    return sessionGrades_;
}

void JuniorStudent::setSessionGrades(const std::vector<int>& grades) {
    if (grades.size() > MAX_JUNIOR_GRADES) {
        throw std::invalid_argument("A junior student can have at most " + std::to_string(MAX_JUNIOR_GRADES) + " grades.");
    }
    sessionGrades_ = grades;
}

} // namespace university 