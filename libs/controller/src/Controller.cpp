#include "Controller.h"

namespace university {

Controller::Controller() : studentTable_(16) {} // Initial capacity

void Controller::run() {
    bool running = true;
    while (running) {
        int choice = view_.showMenu();
        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                findStudent();
                break;
            case 3:
                removeStudent();
                break;
            case 4:
                showAllStudents();
                break;
            case 5:
                running = false;
                break;
            default:
                view_.showMessage("Invalid choice. Please try again.");
        }
    }
}

void Controller::addStudent() {
    try {
        int id = view_.getStudentId();
        if (studentTable_.find(id)) {
            view_.showMessage("Error: Student with this ID already exists.");
            return;
        }
        std::unique_ptr<Student> student = view_.getNewStudentInfo();
        if (student) {
            if (studentTable_.insert(id, std::move(student))) {
                view_.showMessage("Student added successfully.");
            }
        }
    } catch (const std::exception& e) {
        view_.showMessage("Error adding student: " + std::string(e.what()));
    }
}

void Controller::findStudent() {
    int id = view_.getStudentId();
    auto result = studentTable_.find(id);
    if (result) {
        view_.showStudentInfo(*result->get());
    } else {
        view_.showMessage("Student not found.");
    }
}

void Controller::removeStudent() {
    int id = view_.getStudentId();
    if (studentTable_.remove(id)) {
        view_.showMessage("Student removed successfully.");
    } else {
        view_.showMessage("Student not found.");
    }
}

void Controller::showAllStudents() {
    view_.showStudentTable(studentTable_);
}

} // namespace university 