#pragma once

#include "View.h"
#include "HashTable.h"
#include "Student.h"
#include <memory>
#include <map>
#include <thread>
#include <mutex>

namespace university
{

    /**
     * @class Controller
     * @brief Управляет логикой приложения и взаимодействием с пользователем.
     *
     * Класс Controller координирует поток приложения,
     * обрабатывая пользовательский ввод из View и манипулируя данными в Model.
     */
    class Controller
    {
    public:
        /**
         * @brief Конструирует новый объект Controller.
         */
        Controller();

        /**
         * @brief Запускает основной цикл приложения.
         */
        void run();

    private:
        /**
         * @brief Обрабатывает процесс добавления нового студента.
         */
        void addStudent();

        /**
         * @brief Обрабатывает процесс поиска студента по ID.
         */
        void findStudent();

        /**
         * @brief Обрабатывает процесс удаления студента по ID.
         */
        void removeStudent();

        /**
         * @brief Обрабатывает отображение всех студентов в реестре.
         */
        void showAllStudents();

        /**
         * @brief Обрабатывает изменение индекса группы студента.
         */
        void changeStudentGroup();

        /**
         * @brief Обрабатывает перевод студента в другую категорию.
         */
        void transferStudent();

        /**
         * @brief Обрабатывает отображение оценок студента.
         */
        void showStudentGrades();

        /**
         * @brief Обрабатывает отображение средних оценок по группам.
         */
        void showAverageGradesByGroup();

        /**
         * @brief Обрабатывает изменение информации об исследовательской работе.
         */
        void modifyResearchWork();

        /**
         * @brief Вычисляет средние оценки для каждой группы (однопоточная версия).
         * @return Карта индекса группы к средней оценке.
         */
        std::map<std::string, double> calculateAverageGradesByGroup();

        /**
         * @brief Вычисляет средние оценки для каждой группы (многопоточная версия).
         * @return Карта индекса группы к средней оценке.
         */
        std::map<std::string, double> calculateAverageGradesByGroupMultithreaded();

        /**
         * @brief Обрабатывает отображение средних оценок по группам с выбором режима.
         */
        void showAverageGradesByGroupWithChoice();

        View view_;
        HashTable<int, std::unique_ptr<Student>> studentTable_;
        int nextId_ = 1;                // Следующий доступный ID
        mutable std::mutex tableMutex_; // Мьютекс для синхронизации доступа к таблице
    };

} // namespace university