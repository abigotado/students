#include "Controller.h"
#include "JuniorStudent.h"
#include "SeniorStudent.h"
#include "GraduateStudent.h"
#include <algorithm>
#include <numeric>
#include <chrono>
#include <future>
#include <set>

namespace university {

Controller::Controller() : studentTable_(16) {} // Начальная ёмкость

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
                changeStudentGroup();
                break;
            case 6:
                transferStudent();
                break;
            case 7:
                showStudentGrades();
                break;
            case 8:
                showAverageGradesByGroupWithChoice();
                break;
            case 9:
                modifyResearchWork();
                break;
            case 10:
                running = false;
                break;
            default:
                view_.showMessage("Неверный выбор. Пожалуйста, попробуйте еще раз.");
        }
    }
}

void Controller::addStudent() {
    auto student = view_.getNewStudentInfo();
    if (student) {
        std::lock_guard<std::mutex> lock(tableMutex_);
        int id = nextId_++;
        studentTable_.insert(id, std::move(student));
        view_.showMessage("Студент успешно добавлен с ID: " + std::to_string(id));
    } else {
        view_.showMessage("Ошибка при создании студента.");
    }
}

void Controller::findStudent() {
    int id = view_.getStudentId();
    std::lock_guard<std::mutex> lock(tableMutex_);
    auto student = studentTable_.find(id);
    if (student) {
        view_.showStudentInfo(*student->get());
    } else {
        view_.showMessage("Студент с ID " + std::to_string(id) + " не найден.");
    }
}

void Controller::removeStudent() {
    int id = view_.getStudentId();
    std::lock_guard<std::mutex> lock(tableMutex_);
    if (studentTable_.remove(id)) {
        view_.showMessage("Студент с ID " + std::to_string(id) + " успешно удален.");
    } else {
        view_.showMessage("Студент с ID " + std::to_string(id) + " не найден.");
    }
}

void Controller::showAllStudents() {
    std::lock_guard<std::mutex> lock(tableMutex_);
    view_.showStudentTable(studentTable_);
}

void Controller::changeStudentGroup() {
    int id = view_.getStudentId();
    std::lock_guard<std::mutex> lock(tableMutex_);
    auto student = studentTable_.find(id);
    if (!student) {
        view_.showMessage("Студент с ID " + std::to_string(id) + " не найден.");
        return;
    }
    
    std::string newGroup = view_.getNewGroupIndex();
    student->get()->setGroupIndex(newGroup);
    view_.showMessage("Группа студента успешно изменена.");
}

void Controller::transferStudent() {
    int id = view_.getStudentId();
    std::lock_guard<std::mutex> lock(tableMutex_);
    auto student = studentTable_.find(id);
    if (!student) {
        view_.showMessage("Студент с ID " + std::to_string(id) + " не найден.");
        return;
    }
    
    int newCategory = view_.getNewCategory();
    StudentCategory category = static_cast<StudentCategory>(newCategory - 1);
    
    if (student->get()->getCategory() == category) {
        view_.showMessage("Студент уже находится в этой категории.");
        return;
    }
    
    // Создаём нового студента целевой категории
    std::unique_ptr<Student> newStudent;
    switch (category) {
        case StudentCategory::JUNIOR:
            newStudent = std::make_unique<JuniorStudent>(student->get()->getName(), 
                                                       student->get()->getGroupIndex(), 
                                                       student->get()->getDepartmentNumber(),
                                                       std::vector<int>{}); // Пустой вектор оценок
            break;
        case StudentCategory::SENIOR:
            newStudent = std::make_unique<SeniorStudent>(student->get()->getName(), 
                                                       student->get()->getGroupIndex(), 
                                                       student->get()->getDepartmentNumber(),
                                                       std::vector<int>{}, // Пустой вектор оценок
                                                       ResearchWork{"", "", 0, 0}); // Пустая исследовательская работа
            break;
        case StudentCategory::GRADUATE:
            newStudent = std::make_unique<GraduateStudent>(student->get()->getName(), 
                                                         student->get()->getGroupIndex(), 
                                                         student->get()->getDepartmentNumber(),
                                                         DiplomaProject{"", "", 0, 0, 0}); // Пустой дипломный проект
            break;
    }
    
    // Заменяем старого студента новым
    studentTable_.remove(id);
    studentTable_.insert(id, std::move(newStudent));
    view_.showMessage("Студент успешно переведен в новую категорию.");
}

void Controller::showStudentGrades() {
    int id = view_.getStudentId();
    std::lock_guard<std::mutex> lock(tableMutex_);
    auto student = studentTable_.find(id);
    if (student) {
        view_.showStudentGrades(*student->get());
    } else {
        view_.showMessage("Студент с ID " + std::to_string(id) + " не найден.");
    }
}

void Controller::showAverageGradesByGroup() {
    std::lock_guard<std::mutex> lock(tableMutex_);
    auto averages = calculateAverageGradesByGroup();
    view_.showAverageGradesByGroup(averages);
}

void Controller::showAverageGradesByGroupWithChoice() {
    int mode = view_.getAverageCalculationMode();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::map<std::string, double> averages;
    if (mode == 1) {
        std::lock_guard<std::mutex> lock(tableMutex_);
        averages = calculateAverageGradesByGroup();
    } else {
        averages = calculateAverageGradesByGroupMultithreaded();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    double timeMs = duration.count() / 1000.0;
    
    view_.showExecutionTime(mode, timeMs);
    view_.showAverageGradesByGroup(averages);
}

std::map<std::string, double> Controller::calculateAverageGradesByGroup() {
    std::map<std::string, std::vector<double>> groupGrades;
    
    // Собираем все оценки по группам
    for (auto it = studentTable_.begin(); it != studentTable_.end(); ++it) {
        auto pair = *it;
        const auto& student = pair.second;
        std::string group = student->getGroupIndex();
        
        std::vector<double> grades;
        
        switch(student->getCategory()) {
            case StudentCategory::JUNIOR: {
                const auto& junior = dynamic_cast<const JuniorStudent&>(*student);
                for(int grade : junior.getSessionGrades()) {
                    grades.push_back(static_cast<double>(grade));
                }
                break;
            }
            case StudentCategory::SENIOR: {
                const auto& senior = dynamic_cast<const SeniorStudent&>(*student);
                for(int grade : senior.getSessionGrades()) {
                    grades.push_back(static_cast<double>(grade));
                }
                const auto& work = senior.getResearchWork();
                grades.push_back(static_cast<double>(work.supervisorGrade));
                grades.push_back(static_cast<double>(work.commissionGrade));
                break;
            }
            case StudentCategory::GRADUATE: {
                const auto& graduate = dynamic_cast<const GraduateStudent&>(*student);
                const auto& project = graduate.getDiplomaProject();
                grades.push_back(static_cast<double>(project.supervisorGrade));
                grades.push_back(static_cast<double>(project.reviewerGrade));
                grades.push_back(static_cast<double>(project.stateCommissionGrade));
                break;
            }
        }
        
        groupGrades[group].insert(groupGrades[group].end(), grades.begin(), grades.end());
    }
    
    // Вычисляем средние значения
    std::map<std::string, double> averages;
    for (const auto& [group, grades] : groupGrades) {
        if (!grades.empty()) {
            double sum = std::accumulate(grades.begin(), grades.end(), 0.0);
            averages[group] = sum / grades.size();
        }
    }
    
    return averages;
}

std::map<std::string, double> Controller::calculateAverageGradesByGroupMultithreaded() {
    std::lock_guard<std::mutex> lock(tableMutex_);
    
    // Собираем группы
    std::set<std::string> groups;
    for (auto it = studentTable_.begin(); it != studentTable_.end(); ++it) {
        auto pair = *it;
        groups.insert(pair.second->getGroupIndex());
    }
    
    std::map<std::string, double> averages;
    std::mutex averagesMutex;
    
    // Создаём потоки для каждой группы
    std::vector<std::future<void>> futures;
    
    for (const auto& group : groups) {
        futures.emplace_back(std::async(std::launch::async, [&, group]() {
            std::vector<double> grades;
            
            // Собираем оценки для данной группы
            for (auto it = studentTable_.begin(); it != studentTable_.end(); ++it) {
                auto pair = *it;
                const auto& student = pair.second;
                
                if (student->getGroupIndex() != group) continue;
                
                switch(student->getCategory()) {
                    case StudentCategory::JUNIOR: {
                        const auto& junior = dynamic_cast<const JuniorStudent&>(*student);
                        for(int grade : junior.getSessionGrades()) {
                            grades.push_back(static_cast<double>(grade));
                        }
                        break;
                    }
                    case StudentCategory::SENIOR: {
                        const auto& senior = dynamic_cast<const SeniorStudent&>(*student);
                        for(int grade : senior.getSessionGrades()) {
                            grades.push_back(static_cast<double>(grade));
                        }
                        const auto& work = senior.getResearchWork();
                        grades.push_back(static_cast<double>(work.supervisorGrade));
                        grades.push_back(static_cast<double>(work.commissionGrade));
                        break;
                    }
                    case StudentCategory::GRADUATE: {
                        const auto& graduate = dynamic_cast<const GraduateStudent&>(*student);
                        const auto& project = graduate.getDiplomaProject();
                        grades.push_back(static_cast<double>(project.supervisorGrade));
                        grades.push_back(static_cast<double>(project.reviewerGrade));
                        grades.push_back(static_cast<double>(project.stateCommissionGrade));
                        break;
                    }
                }
            }
            
            // Вычисляем среднее для группы
            if (!grades.empty()) {
                double sum = std::accumulate(grades.begin(), grades.end(), 0.0);
                double average = sum / grades.size();
                
                std::lock_guard<std::mutex> lock(averagesMutex);
                averages[group] = average;
            }
        }));
    }
    
    // Ждём завершения всех потоков
    for (auto& future : futures) {
        future.wait();
    }
    
    return averages;
}

void Controller::modifyResearchWork() {
    int id = view_.getStudentId();
    std::lock_guard<std::mutex> lock(tableMutex_);
    auto student = studentTable_.find(id);
    if (!student) {
        view_.showMessage("Студент с ID " + std::to_string(id) + " не найден.");
        return;
    }
    
    if (student->get()->getCategory() != StudentCategory::SENIOR) {
        view_.showMessage("Только старшекурсники могут иметь исследовательскую работу.");
        return;
    }
    
    auto& senior = dynamic_cast<SeniorStudent&>(*student->get());
    ResearchWork newWork = view_.getNewResearchWork();
    senior.setResearchWork(newWork);
    view_.showMessage("Исследовательская работа успешно изменена.");
}

} // namespace university 