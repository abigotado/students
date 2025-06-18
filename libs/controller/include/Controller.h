#pragma once

#include "View.h"
#include "HashTable.h"
#include "Student.h"
#include <memory>

namespace university {

/**
 * @class Controller
 * @brief Manages the application logic and user interaction.
 *
 * The Controller class orchestrates the flow of the application,
 * handling user input from the View and manipulating the data in the Model.
 */
class Controller {
public:
    /**
     * @brief Constructs a new Controller object.
     */
    Controller();

    /**
     * @brief Runs the main application loop.
     */
    void run();

private:
    /**
     * @brief Handles the process of adding a new student.
     */
    void addStudent();

    /**
     * @brief Handles the process of finding a student by ID.
     */
    void findStudent();

    /**
     * @brief Handles the process of removing a student by ID.
     */
    void removeStudent();

    /**
     * @brief Handles displaying all students in the registry.
     */
    void showAllStudents();

    View view_;
    HashTable<int, std::unique_ptr<Student>> studentTable_;
};

} // namespace university 