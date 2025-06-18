#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Student.h"
#include "HashTable.h"

namespace university {

class View {
public:
    /**
     * @brief Displays the main menu and gets the user's choice.
     * @return The user's menu choice.
     */
    int showMenu();

    /**
     * @brief Prompts the user for a student ID.
     * @return The student ID entered by the user.
     */
    int getStudentId();

    /**
     * @brief Prompts the user for information to create a new student.
     * @return A unique_ptr to the newly created student.
     */
    std::unique_ptr<Student> getNewStudentInfo();

    /**
     * @brief Displays the information of a single student.
     * @param student The student to display.
     */
    void showStudentInfo(const Student& student);

    /**
     * @brief Displays the contents of the student table.
     * @param table The hash table of students.
     */
    void showStudentTable(const HashTable<int, std::unique_ptr<Student>>& table);

    /**
     * @brief Displays a message to the user.
     * @param message The message to display.
     */
    void showMessage(const std::string& message);

private:
    /**
     * @brief Prompts for and reads common student information.
     * @param name Reference to store the student's name.
     * @param groupIndex Reference to store the group index.
     * @param departmentNumber Reference to store the department number.
     */
    void promptForCommonInfo(std::string& name, std::string& groupIndex, int& departmentNumber);
};

} // namespace university 