#include "View.h"
#include "JuniorStudent.h"
#include "SeniorStudent.h"
#include "GraduateStudent.h"
#include <limits>
#include <sstream>

namespace university {

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int View::showMenu() {
    std::cout << "\n--- Student Registry Menu ---\n";
    std::cout << "1. Add Student\n";
    std::cout << "2. Find Student\n";
    std::cout << "3. Remove Student\n";
    std::cout << "4. Show All Students\n";
    std::cout << "5. Exit\n";
    std::cout << "Enter your choice: ";
    int choice;
    std::cin >> choice;
    while(std::cin.fail()) {
        std::cout << "Invalid input. Please enter a number." << std::endl;
        std::cin.clear();
        clearInputBuffer();
        std::cin >> choice;
    }
    clearInputBuffer();
    return choice;
}

int View::getStudentId() {
    std::cout << "Enter student ID: ";
    int id;
    std::cin >> id;
    while(std::cin.fail()) {
        std::cout << "Invalid input. Please enter a number." << std::endl;
        std::cin.clear();
        clearInputBuffer();
        std::cin >> id;
    }
    clearInputBuffer();
    return id;
}

void View::promptForCommonInfo(std::string& name, std::string& groupIndex, int& departmentNumber) {
    std::cout << "Enter name: ";
    std::getline(std::cin, name);
    std::cout << "Enter group index: ";
    std::getline(std::cin, groupIndex);
    std::cout << "Enter department number: ";
    std::cin >> departmentNumber;
    while(std::cin.fail()) {
        std::cout << "Invalid input. Please enter a number." << std::endl;
        std::cin.clear();
        clearInputBuffer();
        std::cin >> departmentNumber;
    }
    clearInputBuffer();
}

std::unique_ptr<Student> View::getNewStudentInfo() {
    std::string name, groupIndex;
    int departmentNumber;

    promptForCommonInfo(name, groupIndex, departmentNumber);

    std::cout << "Enter student category (1: Junior, 2: Senior, 3: Graduate): ";
    int category;
    std::cin >> category;
    while(std::cin.fail() || category < 1 || category > 3) {
        std::cout << "Invalid input. Please enter 1, 2, or 3." << std::endl;
        std::cin.clear();
        clearInputBuffer();
        std::cin >> category;
    }
    clearInputBuffer();

    switch (static_cast<StudentCategory>(category - 1)) {
        case StudentCategory::JUNIOR: {
            std::cout << "Enter up to 5 session grades (space-separated): ";
            std::vector<int> grades;
            std::string line;
            std::getline(std::cin, line);
            std::istringstream iss(line);
            int grade;
            while (iss >> grade) {
                grades.push_back(grade);
            }
            return std::make_unique<JuniorStudent>(name, groupIndex, departmentNumber, grades);
        }
        case StudentCategory::SENIOR: {
            std::cout << "Enter up to 4 session grades (space-separated): ";
            std::vector<int> grades;
            std::string line;
            std::getline(std::cin, line);
            std::istringstream iss(line);
            int grade;
            while (iss >> grade) {
                grades.push_back(grade);
            }
            ResearchWork work;
            std::cout << "Enter research work topic: ";
            std::getline(std::cin, work.topic);
            std::cout << "Enter research work place: ";
            std::getline(std::cin, work.place);
            std::cout << "Enter supervisor grade: ";
            std::cin >> work.supervisorGrade;
            std::cout << "Enter commission grade: ";
            std::cin >> work.commissionGrade;
            clearInputBuffer();
            return std::make_unique<SeniorStudent>(name, groupIndex, departmentNumber, grades, work);
        }
        case StudentCategory::GRADUATE: {
            DiplomaProject project;
            std::cout << "Enter diploma project topic: ";
            std::getline(std::cin, project.topic);
            std::cout << "Enter diploma project place: ";
            std::getline(std::cin, project.place);
            std::cout << "Enter supervisor grade: ";
            std::cin >> project.supervisorGrade;
            std::cout << "Enter reviewer grade: ";
            std::cin >> project.reviewerGrade;
            std::cout << "Enter state commission grade: ";
            std::cin >> project.stateCommissionGrade;
            clearInputBuffer();
            return std::make_unique<GraduateStudent>(name, groupIndex, departmentNumber, project);
        }
    }
    return nullptr;
}

void View::showStudentInfo(const Student& student) {
    student.printInfo(std::cout);
}

void View::showStudentTable(const HashTable<int, std::unique_ptr<Student>>& table) {
    std::cout << "\n--- All Students ---\n";
    if (table.empty()) {
        std::cout << "No students in the registry." << std::endl;
        return;
    }
    for (auto it = table.begin(); it != table.end(); ++it) {
        auto pair = *it;
        std::cout << "ID: " << pair.first << std::endl;
        pair.second->printInfo(std::cout);
        std::cout << "---------------------\n";
    }
}

void View::showMessage(const std::string& message) {
    std::cout << message << std::endl;
}

} // namespace university 