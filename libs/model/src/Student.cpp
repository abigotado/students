#include "Student.h"
#include <stdexcept>

namespace university {

Student::Student(const std::string& name, const std::string& groupIndex, int departmentNumber)
    : name_(name), groupIndex_(groupIndex), departmentNumber_(departmentNumber) {
    if (name.empty()) {
        throw std::invalid_argument("Student name cannot be empty.");
    }
    if (departmentNumber < 0) {
        throw std::invalid_argument("Department number cannot be negative.");
    }
}

void Student::printInfo(std::ostream& os) const {
    os << "Name: " << name_ << std::endl;
    os << "Group Index: " << groupIndex_ << std::endl;
    os << "Department Number: " << departmentNumber_ << std::endl;
}

void Student::setGroupIndex(const std::string& newGroupIndex) {
    groupIndex_ = newGroupIndex;
}

} // namespace university 