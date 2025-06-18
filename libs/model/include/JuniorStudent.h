#pragma once

#include "Student.h"
#include <vector>

namespace university {

/**
 * @class JuniorStudent
 * @brief Представляет младшекурсника.
 */
class JuniorStudent : public Student {
public:
    /**
     * @brief Конструирует новый объект JuniorStudent.
     * @param name Полное имя студента.
     * @param groupIndex Индекс группы студента.
     * @param departmentNumber Номер кафедры студента.
     * @param sessionGrades Вектор оценок за последнюю сессию (максимум 5).
     * @throw std::invalid_argument если sessionGrades содержит более 5 оценок.
     */
    JuniorStudent(const std::string& name, const std::string& groupIndex, int departmentNumber, const std::vector<int>& sessionGrades);

    /**
     * @brief Получает категорию студента.
     * @return Всегда возвращает StudentCategory::JUNIOR.
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
     * @param grades Вектор оценок (максимум 5).
     * @throw std::invalid_argument если grades содержит более 5 оценок.
     */
    void setSessionGrades(const std::vector<int>& grades);

private:
    std::vector<int> sessionGrades_;
};

} // namespace university 