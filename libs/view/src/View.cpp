#include "View.h"
#include "JuniorStudent.h"
#include "SeniorStudent.h"
#include "GraduateStudent.h"
#include <limits>
#include <sstream>
#include <iomanip>

namespace university
{

    void View::clearInputBuffer()
    {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    int View::showMenu()
    {
        std::cout << "\n--- Реестр студентов ---\n";
        std::cout << "1. Добавить студента\n";
        std::cout << "2. Найти студента\n";
        std::cout << "3. Удалить студента\n";
        std::cout << "4. Показать всех студентов\n";
        std::cout << "5. Изменить группу студента\n";
        std::cout << "6. Перевести студента\n";
        std::cout << "7. Показать оценки студента\n";
        std::cout << "8. Показать средние оценки по группам\n";
        std::cout << "9. Изменить исследовательскую работу\n";
        std::cout << "10. Выход\n";
        std::cout << "Введите ваш выбор: ";
        int choice;
        std::cin >> choice;
        while (std::cin.fail())
        {
            std::cout << "Неверный ввод. Пожалуйста, введите число." << std::endl;
            std::cin.clear();
            clearInputBuffer();
            std::cin >> choice;
        }
        clearInputBuffer();
        return choice;
    }

    int View::getStudentId()
    {
        std::cout << "Введите ID студента: ";
        int id;
        std::cin >> id;
        while (std::cin.fail())
        {
            std::cout << "Неверный ввод. Пожалуйста, введите число." << std::endl;
            std::cin.clear();
            clearInputBuffer();
            std::cin >> id;
        }
        clearInputBuffer();
        return id;
    }

    void View::promptForCommonInfo(std::string &name, std::string &groupIndex, int &departmentNumber)
    {
        std::cout << "Введите имя: ";
        std::getline(std::cin, name);
        std::cout << "Введите индекс группы: ";
        std::getline(std::cin, groupIndex);
        std::cout << "Введите номер кафедры: ";
        std::cin >> departmentNumber;
        while (std::cin.fail())
        {
            std::cout << "Неверный ввод. Пожалуйста, введите число." << std::endl;
            std::cin.clear();
            clearInputBuffer();
            std::cin >> departmentNumber;
        }
        clearInputBuffer();
    }

    std::unique_ptr<Student> View::getNewStudentInfo()
    {
        std::string name, groupIndex;
        int departmentNumber;

        promptForCommonInfo(name, groupIndex, departmentNumber);

        std::cout << "Введите категорию студента (1: Младшекурсник, 2: Старшекурсник, 3: Выпускник): ";
        int category;
        std::cin >> category;
        while (std::cin.fail() || category < 1 || category > 3)
        {
            std::cout << "Неверный ввод. Пожалуйста, введите 1, 2 или 3." << std::endl;
            std::cin.clear();
            clearInputBuffer();
            std::cin >> category;
        }
        clearInputBuffer();

        switch (static_cast<StudentCategory>(category - 1))
        {
        case StudentCategory::JUNIOR:
        case StudentCategory::SENIOR:
        case StudentCategory::GRADUATE:
            break;
        default:
            break;
        }

        switch (static_cast<StudentCategory>(category - 1))
        {
        case StudentCategory::JUNIOR:
        {
            std::cout << "Введите до 5 оценок за сессию (через пробел): ";
            std::vector<int> grades;
            std::string line;
            std::getline(std::cin, line);
            std::istringstream iss(line);
            int grade;
            while (iss >> grade)
            {
                grades.push_back(grade);
            }
            return std::make_unique<JuniorStudent>(name, groupIndex, departmentNumber, grades);
        }
        case StudentCategory::SENIOR:
        {
            std::cout << "Введите до 4 оценок за сессию (через пробел): ";
            std::vector<int> grades;
            std::string line;
            std::getline(std::cin, line);
            std::istringstream iss(line);
            int grade;
            while (iss >> grade)
            {
                grades.push_back(grade);
            }
            ResearchWork work;
            std::cout << "Введите тему УИР: ";
            std::getline(std::cin, work.topic);
            std::cout << "Введите место выполнения УИР: ";
            std::getline(std::cin, work.place);
            std::cout << "Введите оценку руководителя: ";
            std::cin >> work.supervisorGrade;
            std::cout << "Введите оценку комиссии: ";
            std::cin >> work.commissionGrade;
            clearInputBuffer();
            return std::make_unique<SeniorStudent>(name, groupIndex, departmentNumber, grades, work);
        }
        case StudentCategory::GRADUATE:
        {
            DiplomaProject project;
            std::cout << "Введите тему дипломного проекта: ";
            std::getline(std::cin, project.topic);
            std::cout << "Введите место выполнения ДП: ";
            std::getline(std::cin, project.place);
            std::cout << "Введите оценку руководителя: ";
            std::cin >> project.supervisorGrade;
            std::cout << "Введите оценку рецензента: ";
            std::cin >> project.reviewerGrade;
            std::cout << "Введите оценку ГЭК: ";
            std::cin >> project.stateCommissionGrade;
            clearInputBuffer();
            return std::make_unique<GraduateStudent>(name, groupIndex, departmentNumber, project);
        }
        }
        return nullptr;
    }

    void View::showStudentInfo(const Student &student)
    {
        student.printInfo(std::cout);
    }

    void View::showStudentTable(const HashTable<int, std::unique_ptr<Student>> &table)
    {
        std::cout << "\n--- Все студенты ---\n";
        if (table.size() == 0)
        {
            std::cout << "В реестре нет студентов." << std::endl;
            return;
        }
        for (auto it = table.begin(); it != table.end(); ++it)
        {
            auto pair = *it;
            std::cout << "ID: " << pair.first << std::endl;
            pair.second->printInfo(std::cout);
            std::cout << "---------------------\n";
        }
    }

    void View::showMessage(const std::string &message)
    {
        std::cout << message << std::endl;
    }

    std::string View::getNewGroupIndex()
    {
        std::cout << "Введите новый индекс группы: ";
        std::string groupIndex;
        std::getline(std::cin, groupIndex);
        return groupIndex;
    }

    int View::getNewCategory()
    {
        std::cout << "Введите новую категорию (1: Младшекурсник, 2: Старшекурсник, 3: Выпускник): ";
        int category;
        std::cin >> category;
        while (std::cin.fail() || category < 1 || category > 3)
        {
            std::cout << "Неверный ввод. Пожалуйста, введите 1, 2 или 3." << std::endl;
            std::cin.clear();
            clearInputBuffer();
            std::cin >> category;
        }
        clearInputBuffer();
        return category;
    }

    void View::showStudentGrades(const Student &student)
    {
        std::cout << "\n--- Оценки студента ---\n";
        std::cout << "Имя: " << student.getName() << std::endl;
        std::cout << "Категория: ";
        switch (student.getCategory())
        {
        case StudentCategory::JUNIOR:
        case StudentCategory::SENIOR:
        case StudentCategory::GRADUATE:
            break;
        default:
            break;
        }
        std::cout << std::endl;

        // Отображаем оценки в зависимости от категории
        switch (student.getCategory())
        {
        case StudentCategory::JUNIOR:
        {
            const auto &junior = dynamic_cast<const JuniorStudent &>(student);
            std::cout << "Оценки за сессию: ";
            for (int grade : junior.getSessionGrades())
            {
                std::cout << grade << " ";
            }
            std::cout << std::endl;
            break;
        }
        case StudentCategory::SENIOR:
        {
            const auto &senior = dynamic_cast<const SeniorStudent &>(student);
            std::cout << "Оценки за сессию: ";
            for (int grade : senior.getSessionGrades())
            {
                std::cout << grade << " ";
            }
            std::cout << std::endl;
            const auto &work = senior.getResearchWork();
            std::cout << "Оценки за УИР (Руководитель, Комиссия): "
                      << work.supervisorGrade << ", " << work.commissionGrade << std::endl;
            break;
        }
        case StudentCategory::GRADUATE:
        {
            const auto &graduate = dynamic_cast<const GraduateStudent &>(student);
            const auto &project = graduate.getDiplomaProject();
            std::cout << "Оценки за ДП (Руководитель, Рецензент, ГЭК): "
                      << project.supervisorGrade << ", " << project.reviewerGrade
                      << ", " << project.stateCommissionGrade << std::endl;
            break;
        }
        }
    }

    void View::showAverageGradesByGroup(const std::map<std::string, double> &averages)
    {
        std::cout << "\n--- Средние оценки по группам ---\n";
        if (averages.empty())
        {
            std::cout << "Студенты не найдены." << std::endl;
            return;
        }
        for (const auto &[group, average] : averages)
        {
            std::cout << "Группа " << group << ": " << std::fixed << std::setprecision(2) << average << std::endl;
        }
    }

    ResearchWork View::getNewResearchWork()
    {
        ResearchWork work;
        std::cout << "Введите тему УИР: ";
        std::getline(std::cin, work.topic);
        std::cout << "Введите место выполнения УИР: ";
        std::getline(std::cin, work.place);
        std::cout << "Введите оценку руководителя: ";
        std::cin >> work.supervisorGrade;
        std::cout << "Введите оценку комиссии: ";
        std::cin >> work.commissionGrade;
        clearInputBuffer();
        return work;
    }

    DiplomaProject View::getNewDiplomaProject()
    {
        DiplomaProject project;
        std::cout << "Введите тему дипломного проекта: ";
        std::getline(std::cin, project.topic);
        std::cout << "Введите место выполнения ДП: ";
        std::getline(std::cin, project.place);
        std::cout << "Введите оценку руководителя: ";
        std::cin >> project.supervisorGrade;
        std::cout << "Введите оценку рецензента: ";
        std::cin >> project.reviewerGrade;
        std::cout << "Введите оценку ГЭК: ";
        std::cin >> project.stateCommissionGrade;
        clearInputBuffer();
        return project;
    }

    int View::getAverageCalculationMode()
    {
        std::cout << "\nВыберите режим вычисления средних оценок:\n";
        std::cout << "1. Однопоточный режим\n";
        std::cout << "2. Многопоточный режим\n";
        std::cout << "Введите ваш выбор: ";
        int choice;
        std::cin >> choice;
        while (std::cin.fail() || choice < 1 || choice > 2)
        {
            std::cout << "Неверный ввод. Пожалуйста, введите 1 или 2." << std::endl;
            std::cin.clear();
            clearInputBuffer();
            std::cin >> choice;
        }
        clearInputBuffer();
        return choice;
    }

    void View::showExecutionTime(int mode, double timeMs)
    {
        std::string modeStr = (mode == 1) ? "однопоточном" : "многопоточном";
        std::cout << "Время выполнения в " << modeStr << " режиме: "
                  << std::fixed << std::setprecision(3) << timeMs << " мс" << std::endl;
    }

} // namespace university