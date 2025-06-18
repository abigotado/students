#pragma once

#include "Student.h"
#include <vector>
#include <string>

namespace university {

/**
 * @struct ResearchWork
 * @brief Представляет исследовательскую работу студента.
 */
struct ResearchWork {
    std::string topic;
    std::string place;
    int supervisorGrade;
    int commissionGrade;
};

/**
 * @class SeniorStudent
 * @brief Представляет старшекурсника.
 */
class SeniorStudent : public Student {
public:
    /**
     * @brief Конструирует новый объект SeniorStudent.
     * @param name Полное имя студента.
     * @param groupIndex Индекс группы студента.
     * @param departmentNumber Номер кафедры студента.
     * @param sessionGrades Вектор оценок за последнюю сессию (максимум 4).
     * @param researchWork Детали исследовательской работы студента.
     * @throw std::invalid_argument если sessionGrades содержит более 4 оценок.
     */
    SeniorStudent(const std::string& name, const std::string& groupIndex, int departmentNumber,
                  const std::vector<int>& sessionGrades, const ResearchWork& researchWork);

    /**
     * @brief Получает категорию студента.
     * @return Всегда возвращает StudentCategory::SENIOR.
     */
    [[nodiscard]] StudentCategory getCategory() const override;

    /**
     * @brief Выводит информацию о студенте в заданный поток вывода.
     * @param os Поток вывода для записи.
     */
    void printInfo(std::ostream& os) const override;

    /**
     * @brief Получает оценки за последнюю сессию.
     * @return Константная ссылка на вектор оценок.
     */
    [[nodiscard]] const std::vector<int>& getSessionGrades() const;

    /**
     * @brief Устанавливает оценки за последнюю сессию.
     * @param grades Вектор оценок (максимум 4).
     * @throw std::invalid_argument если grades содержит более 4 оценок.
     */
    void setSessionGrades(const std::vector<int>& grades);

    /**
     * @brief Получает информацию об исследовательской работе студента.
     * @return Константная ссылка на структуру исследовательской работы.
     */
    [[nodiscard]] const ResearchWork& getResearchWork() const;

    /**
     * @brief Устанавливает информацию об исследовательской работе студента.
     * @param researchWork Новые детали исследовательской работы.
     */
    void setResearchWork(const ResearchWork& researchWork);


private:
    std::vector<int> sessionGrades_;
    ResearchWork researchWork_;
};

} // namespace university 