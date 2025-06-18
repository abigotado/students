#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Student.h"
#include "HashTable.h"
#include "SeniorStudent.h"
#include "GraduateStudent.h"

namespace university
{

    class View
    {
    public:
        /**
         * @brief Отображает главное меню и получает выбор пользователя.
         * @return Выбор пользователя из меню.
         */
        int showMenu();

        /**
         * @brief Запрашивает у пользователя ID студента.
         * @return ID студента, введённый пользователем.
         */
        int getStudentId();

        /**
         * @brief Запрашивает у пользователя информацию для создания нового студента.
         * @return unique_ptr к новому студенту.
         */
        std::unique_ptr<Student> getNewStudentInfo();

        /**
         * @brief Отображает информацию об одном студенте.
         * @param student Студент для отображения.
         */
        void showStudentInfo(const Student &student);

        /**
         * @brief Отображает содержимое таблицы студентов.
         * @param table Хеш-таблица студентов.
         */
        void showStudentTable(const HashTable<int, std::unique_ptr<Student>> &table);

        /**
         * @brief Отображает сообщение пользователю.
         * @param message Сообщение для отображения.
         */
        void showMessage(const std::string &message);

        /**
         * @brief Запрашивает новый индекс группы.
         * @return Новый индекс группы.
         */
        std::string getNewGroupIndex();

        /**
         * @brief Запрашивает новую категорию студента.
         * @return Новая категория (1: Младшекурсник, 2: Старшекурсник, 3: Выпускник).
         */
        int getNewCategory();

        /**
         * @brief Отображает оценки студента.
         * @param student Студент, чьи оценки нужно отобразить.
         */
        void showStudentGrades(const Student &student);

        /**
         * @brief Отображает средние оценки по группам.
         * @param averages Карта индекса группы к средней оценке.
         */
        void showAverageGradesByGroup(const std::map<std::string, double> &averages);

        /**
         * @brief Запрашивает выбор режима вычисления средних оценок.
         * @return 1 для однопоточного режима, 2 для многопоточного.
         */
        int getAverageCalculationMode();

        /**
         * @brief Отображает информацию о времени выполнения.
         * @param mode Режим выполнения (1 - однопоточный, 2 - многопоточный).
         * @param timeMs Время выполнения в миллисекундах.
         */
        void showExecutionTime(int mode, double timeMs);

        /**
         * @brief Запрашивает информацию о новой исследовательской работе.
         * @return Детали новой исследовательской работы.
         */
        ResearchWork getNewResearchWork();

        /**
         * @brief Запрашивает информацию о новом дипломном проекте.
         * @return Детали нового дипломного проекта.
         */
        DiplomaProject getNewDiplomaProject();

    private:
        /**
         * @brief Очищает буфер ввода.
         */
        void clearInputBuffer();

        /**
         * @brief Запрашивает и считывает общую информацию о студенте.
         * @param name Ссылка для хранения имени студента.
         * @param groupIndex Ссылка для хранения индекса группы.
         * @param departmentNumber Ссылка для хранения номера кафедры.
         */
        void promptForCommonInfo(std::string &name, std::string &groupIndex, int &departmentNumber);
    };

} // namespace university