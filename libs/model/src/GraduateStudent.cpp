#include "GraduateStudent.h"

namespace university
{

    GraduateStudent::GraduateStudent(const std::string &name, const std::string &groupIndex, int departmentNumber,
                                     const DiplomaProject &diplomaProject)
        : Student(name, groupIndex, departmentNumber), diplomaProject_(diplomaProject) {}

    StudentCategory GraduateStudent::getCategory() const
    {
        return StudentCategory::GRADUATE;
    }

    void GraduateStudent::printInfo(std::ostream &os) const
    {
        Student::printInfo(os);
        os << "Category: Graduate Student" << std::endl;
        os << "Diploma Project Topic: " << diplomaProject_.topic << std::endl;
        os << "Diploma Project Place: " << diplomaProject_.place << std::endl;
        os << "Diploma Project Grades (Supervisor, Reviewer, State Commission): "
           << diplomaProject_.supervisorGrade << ", "
           << diplomaProject_.reviewerGrade << ", "
           << diplomaProject_.stateCommissionGrade << std::endl;
    }

    const DiplomaProject &GraduateStudent::getDiplomaProject() const
    {
        return diplomaProject_;
    }

    void GraduateStudent::setDiplomaProject(const DiplomaProject &diplomaProject)
    {
        diplomaProject_ = diplomaProject;
    }

} // namespace university