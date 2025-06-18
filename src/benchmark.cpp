#include "Controller.h"
#include "Student.h"
#include "JuniorStudent.h"
#include "SeniorStudent.h"
#include "GraduateStudent.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <random>
#include <thread>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <filesystem>

namespace {
    // Генератор случайных имён
    std::string generateRandomName(std::mt19937& gen) {
        static const std::vector<std::string> surnames = {
            "Иванов", "Петров", "Сидоров", "Козлов", "Новиков", "Морозов", "Петренко", "Иваненко",
            "Шевченко", "Бондаренко", "Мельник", "Романенко", "Павленко", "Марченко", "Ткаченко",
            "Романюк", "Василенко", "Полтавец", "Кравченко", "Олейник", "Бондарь", "Мазепа", "Тарасенко",
            "Лисенко", "Мельниченко", "Дмитренко", "Коваленко", "Шевчук", "Бондарчук", "Пономаренко"
        };
        static const std::vector<std::string> names = {
            "Александр", "Дмитрий", "Максим", "Сергей", "Андрей", "Алексей", "Артём", "Илья",
            "Кирилл", "Михаил", "Никита", "Матвей", "Роман", "Егор", "Арсений", "Владислав",
            "Денис", "Степан", "Владимир", "Данил", "Евгений", "Тимофей", "Владислав", "Игорь",
            "Артём", "Руслан", "Виталий", "Николай", "Павел", "Ростислав", "Глеб", "Константин"
        };
        
        std::uniform_int_distribution<> surnameDist(0, static_cast<int>(surnames.size()) - 1);
        std::uniform_int_distribution<> nameDist(0, static_cast<int>(names.size()) - 1);
        
        return surnames[surnameDist(gen)] + " " + names[nameDist(gen)];
    }
    
    // Генератор случайных групп
    std::string generateRandomGroup(std::mt19937& gen) {
        static const std::vector<std::string> faculties = {"IU", "IT", "IB", "IM", "IE"};
        static const std::vector<std::string> years = {"1", "2", "3", "4", "5"};
        static const std::vector<std::string> groups = {"A", "B", "C", "D", "E"};
        
        std::uniform_int_distribution<> facultyDist(0, static_cast<int>(faculties.size()) - 1);
        std::uniform_int_distribution<> yearDist(0, static_cast<int>(years.size()) - 1);
        std::uniform_int_distribution<> groupDist(0, static_cast<int>(groups.size()) - 1);
        std::uniform_int_distribution<> numberDist(1, 99);
        
        return faculties[facultyDist(gen)] + years[yearDist(gen)] + "-" + 
               std::to_string(numberDist(gen)) + groups[groupDist(gen)];
    }
    
    // Генератор случайных оценок
    std::vector<int> generateRandomGrades(std::mt19937& gen, int count) {
        std::vector<int> grades;
        grades.reserve(static_cast<size_t>(count));
        
        std::uniform_int_distribution<> gradeDist(2, 5);
        
        for (int i = 0; i < count; ++i) {
            grades.push_back(gradeDist(gen));
        }
        
        return grades;
    }
    
    // Генератор исследовательской работы для старшекурсников
    university::ResearchWork generateRandomResearchWork(std::mt19937& gen) {
        static const std::vector<std::string> topics = {
            "Разработка системы управления базами данных",
            "Анализ алгоритмов машинного обучения",
            "Создание веб-приложения для электронной коммерции",
            "Исследование методов криптографической защиты",
            "Разработка мобильного приложения для здравоохранения",
            "Анализ больших данных в социальных сетях",
            "Создание системы искусственного интеллекта",
            "Разработка облачной платформы",
            "Исследование методов компьютерного зрения",
            "Создание системы интернета вещей"
        };
        static const std::vector<std::string> places = {
            "Google", "Microsoft", "Apple", "Amazon", "Meta", "Netflix", "Uber", "Airbnb",
            "Spotify", "Twitter", "LinkedIn", "Salesforce", "Adobe", "Oracle", "IBM",
            "Intel", "NVIDIA", "AMD", "Cisco", "VMware", "Dell", "HP", "Lenovo"
        };
        
        std::uniform_int_distribution<> topicDist(0, static_cast<int>(topics.size()) - 1);
        std::uniform_int_distribution<> placeDist(0, static_cast<int>(places.size()) - 1);
        std::uniform_int_distribution<> gradeDist(3, 5);
        
        return university::ResearchWork{
            gradeDist(gen),
            gradeDist(gen),
            topics[topicDist(gen)],
            places[placeDist(gen)]
        };
    }
    
    // Генератор дипломного проекта для выпускников
    university::DiplomaProject generateRandomDiplomaProject(std::mt19937& gen) {
        static const std::vector<std::string> topics = {
            "Исследование квантовых алгоритмов для оптимизации",
            "Разработка новых методов глубокого обучения",
            "Анализ сложности алгоритмов в теории графов",
            "Создание систем распределённого искусственного интеллекта",
            "Исследование методов защиты от кибератак",
            "Разработка алгоритмов для обработки естественного языка",
            "Анализ эффективности параллельных вычислений",
            "Создание систем компьютерного зрения для робототехники",
            "Исследование методов сжатия данных",
            "Разработка алгоритмов для биоинформатики"
        };
        static const std::vector<std::string> places = {
            "MIT", "Stanford", "Harvard", "Berkeley", "CMU", "Princeton", "Yale", "Columbia",
            "UCLA", "UCSD", "Georgia Tech", "UIUC", "UMich", "UW", "UT Austin", "Cornell",
            "Brown", "Dartmouth", "UPenn", "Northwestern", "Duke", "Vanderbilt", "Rice"
        };
        
        std::uniform_int_distribution<> topicDist(0, static_cast<int>(topics.size()) - 1);
        std::uniform_int_distribution<> placeDist(0, static_cast<int>(places.size()) - 1);
        std::uniform_int_distribution<> gradeDist(3, 5);
        
        return university::DiplomaProject{
            gradeDist(gen),
            gradeDist(gen),
            gradeDist(gen),
            topics[topicDist(gen)],
            places[placeDist(gen)]
        };
    }
    
    // Создание случайного студента
    std::unique_ptr<university::Student> createRandomStudent(std::mt19937& gen, int id) {
        std::uniform_int_distribution<> categoryDist(0, 2);
        university::StudentCategory category = static_cast<university::StudentCategory>(categoryDist(gen));
        
        std::string name = generateRandomName(gen);
        std::string group = generateRandomGroup(gen);
        int department = 100 + (id % 20);
        
        switch (category) {
            case university::StudentCategory::JUNIOR: {
                auto grades = generateRandomGrades(gen, 5);
                return std::make_unique<university::JuniorStudent>(name, group, department, grades);
            }
            case university::StudentCategory::SENIOR: {
                auto grades = generateRandomGrades(gen, 4);
                auto researchWork = generateRandomResearchWork(gen);
                return std::make_unique<university::SeniorStudent>(name, group, department, grades, researchWork);
            }
            case university::StudentCategory::GRADUATE: {
                auto diplomaProject = generateRandomDiplomaProject(gen);
                return std::make_unique<university::GraduateStudent>(name, group, department, diplomaProject);
            }
            default:
                return nullptr;
        }
    }
    
    // Функция для выполнения бенчмарка на определённом объёме данных
    void runBenchmarkForSize(int totalStudents, std::ofstream& csvFile) {
        std::cout << "\n=== Тестирование на " << totalStudents << " студентах ===" << std::endl;
        
        university::Controller controller;
        std::mt19937 gen(42); // Фиксированное зерно для воспроизводимости
        
        // Генерация студентов
        std::cout << "Генерация " << totalStudents << " студентов..." << std::endl;
        for (int i = 1; i <= totalStudents; ++i) {
            auto student = createRandomStudent(gen, i);
            if (student) {
                controller.getStudentTable().insert(i, std::move(student));
            }
        }
        
        // Тест однопоточного режима
        std::cout << "Запуск однопоточного режима..." << std::endl;
        auto startSingle = std::chrono::high_resolution_clock::now();
        
        auto resultSingle = controller.calculateAverageGradesByGroup();
        
        auto endSingle = std::chrono::high_resolution_clock::now();
        auto durationSingle = std::chrono::duration_cast<std::chrono::microseconds>(endSingle - startSingle);
        
        // Тест многопоточного режима
        std::cout << "Запуск многопоточного режима..." << std::endl;
        auto startMulti = std::chrono::high_resolution_clock::now();
        
        auto resultMulti = controller.calculateAverageGradesByGroupMultithreaded();
        
        auto endMulti = std::chrono::high_resolution_clock::now();
        auto durationMulti = std::chrono::duration_cast<std::chrono::microseconds>(endMulti - startMulti);
        
        // Проверка корректности
        bool resultsMatch = (resultSingle.size() == resultMulti.size());
        if (resultsMatch) {
            for (const auto& pair : resultSingle) {
                if (resultMulti.find(pair.first) == resultMulti.end() || 
                    std::abs(resultMulti.at(pair.first) - pair.second) > 0.001) {
                    resultsMatch = false;
                    break;
                }
            }
        }
        
        if (resultsMatch) {
            std::cout << "[OK] Результаты совпадают!" << std::endl;
        } else {
            std::cout << "[ОШИБКА] Результаты не совпадают!" << std::endl;
        }
        
        double timeSingle = durationSingle.count() / 1000.0; // мс
        double timeMulti = durationMulti.count() / 1000.0;   // мс
        double speedup = timeSingle / timeMulti;
        
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "  Однопоточный режим: " << timeSingle << " мс" << std::endl;
        std::cout << "  Многопоточный режим: " << timeMulti << " мс" << std::endl;
        std::cout << "  Ускорение: " << speedup << "x" << std::endl;
        
        // Запись в CSV
        csvFile << totalStudents << "," << timeSingle << "," << timeMulti << "," << speedup << std::endl;
    }
}

int main() {
    std::cout << "=== Бенчмарк производительности студенческого реестра ===" << std::endl;
    std::cout << "Тестирование различных объёмов данных..." << std::endl;
    
    // Создание папки docs, если её нет
    std::filesystem::path docsPath = "../docs";
    if (!std::filesystem::exists(docsPath)) {
        std::filesystem::create_directories(docsPath);
        std::cout << "Создана папка docs" << std::endl;
    }
    
    // Создание CSV файла в папке docs
    std::filesystem::path csvPath = docsPath / "benchmark_results.csv";
    std::ofstream csvFile(csvPath);
    csvFile << "Students,SingleThread(ms),MultiThread(ms),Speedup" << std::endl;
    
    // Различные объёмы данных для тестирования
    std::vector<int> testSizes = {
        100,      // Маленький объём
        500,      // Небольшой объём
        1000,     // Средний объём
        5000,     // Большой объём
        10000,    // Очень большой объём
        50000,    // Огромный объём
        100000,   // Массивный объём
        500000,   // Гигантский объём
        1000000   // Мега объём
    };
    
    for (int size : testSizes) {
        runBenchmarkForSize(size, csvFile);
    }
    
    csvFile.close();
    
    std::cout << "\n=== Бенчмарк завершён ===" << std::endl;
    std::cout << "Результаты сохранены в файл: " << csvPath << std::endl;
    std::cout << "Для построения графика запустите: python3 ../scripts/plot_benchmark.py" << std::endl;
    
    return 0;
} 