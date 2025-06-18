#pragma once

#include "Student.h"
#include <string>

namespace university {

/**
 * @struct DiplomaProject
 * @brief Представляет дипломный проект выпускника.
 */
struct DiplomaProject {
    std::string topic;
    std::string place;
    int supervisorGrade;
    int reviewerGrade;
    int stateCommissionGrade;
};

/**
 * @class GraduateStudent
 * @brief Представляет выпускника.
 */
class GraduateStudent : public Student {
public:
    /**
     * @brief Конструирует новый объект GraduateStudent.
     * @param name Полное имя студента.
     * @param groupIndex Индекс группы студента.
     * @param departmentNumber Номер кафедры студента.
     * @param diplomaProject Детали дипломного проекта студента.
     */
    GraduateStudent(const std::string& name, const std::string& groupIndex, int departmentNumber,
                    const DiplomaProject& diplomaProject);

    /**
     * @brief Получает категорию студента.
     * @return Всегда возвращает StudentCategory::GRADUATE.
     */
    [[nodiscard]] StudentCategory getCategory() const override;

    /**
     * @brief Выводит информацию о студенте в заданный поток вывода.
     * @param os Поток вывода для записи.
     */
    void printInfo(std::ostream& os) const override;

    /**
     * @brief Получает информацию о дипломном проекте студента.
     * @return Константная ссылка на структуру дипломного проекта.
     */
    [[nodiscard]] const DiplomaProject& getDiplomaProject() const;

    /**
     * @brief Устанавливает информацию о дипломном проекте студента.
     * @param project Новые детали дипломного проекта.
     */
    void setDiplomaProject(const DiplomaProject& project);

private:
    DiplomaProject diplomaProject_;
};

} // namespace university 