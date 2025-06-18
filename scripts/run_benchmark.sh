#!/bin/bash

# Скрипт для запуска бенчмарка и построения графика
# Автор: Система реестра студентов
# Версия: 1.0

set -e  # Остановка при ошибке

# Цвета для вывода
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Функция для вывода сообщений
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Проверка зависимостей
check_dependencies() {
    print_info "Проверка зависимостей..."
    
    # Проверка Python
    if ! command -v python3 &> /dev/null; then
        print_error "Python3 не найден. Установите Python3 для построения графика."
        return 1
    fi
    
    # Проверка matplotlib
    if ! python3 -c "import matplotlib" &> /dev/null; then
        print_warning "matplotlib не установлен. Устанавливаем..."
        pip3 install matplotlib pandas numpy
    fi
    
    # Проверка pandas
    if ! python3 -c "import pandas" &> /dev/null; then
        print_warning "pandas не установлен. Устанавливаем..."
        pip3 install pandas
    fi
    
    print_success "Все зависимости установлены"
}

# Сборка проекта
build_project() {
    print_info "Сборка проекта..."
    
    if [ ! -d "build" ]; then
        mkdir build
    fi
    
    cd build
    
    if ! cmake ..; then
        print_error "Ошибка конфигурации CMake"
        exit 1
    fi
    
    if ! make -j$(nproc); then
        print_error "Ошибка сборки"
        exit 1
    fi
    
    cd ..
    print_success "Проект успешно собран"
}

# Запуск бенчмарка
run_benchmark() {
    print_info "Запуск бенчмарка..."
    
    if [ ! -f "build/src/benchmark" ]; then
        print_error "Исполняемый файл бенчмарка не найден"
        exit 1
    fi
    
    # Переходим в корневую директорию проекта
    cd "$(dirname "$0")/.."
    
    # Запускаем бенчмарк в автоматическом режиме
    echo "2" | ./build/src/benchmark
    
    if [ ! -f "benchmark_results.csv" ]; then
        print_error "Файл с результатами бенчмарка не создан"
        exit 1
    fi
    
    print_success "Бенчмарк завершен. Результаты сохранены в benchmark_results.csv"
}

# Построение графика
plot_results() {
    print_info "Построение графика..."
    
    if [ ! -f "benchmark_results.csv" ]; then
        print_error "Файл с результатами не найден"
        exit 1
    fi
    
    if python3 scripts/plot_benchmark.py; then
        print_success "График построен и сохранен в benchmark_results.png"
    else
        print_error "Ошибка при построении графика"
        exit 1
    fi
}

# Показ результатов
show_results() {
    print_info "Результаты бенчмарка:"
    echo "=========================="
    
    if [ -f "benchmark_results.csv" ]; then
        echo "CSV файл с результатами: benchmark_results.csv"
        echo "Первые несколько строк:"
        head -5 benchmark_results.csv
        echo ""
    fi
    
    if [ -f "benchmark_results.png" ]; then
        echo "График: benchmark_results.png"
        echo "Откройте файл для просмотра графика"
    fi
}

# Главная функция
main() {
    echo "=========================================="
    echo "  БЕНЧМАРК ПРОИЗВОДИТЕЛЬНОСТИ"
    echo "  Реестр студентов - Домашнее задание №5"
    echo "=========================================="
    echo ""
    
    # Проверяем, что мы в корневой директории проекта
    if [ ! -f "CMakeLists.txt" ]; then
        print_error "Скрипт должен быть запущен из корневой директории проекта"
        exit 1
    fi
    
    # Выполняем все этапы
    check_dependencies
    build_project
    run_benchmark
    plot_results
    show_results
    
    echo ""
    print_success "Бенчмарк полностью завершен!"
    echo ""
    echo "Файлы результатов:"
    echo "  - benchmark_results.csv - данные в формате CSV"
    echo "  - benchmark_results.png - график производительности"
    echo ""
    echo "Для просмотра графика откройте файл benchmark_results.png"
}

# Обработка аргументов командной строки
case "${1:-}" in
    --help|-h)
        echo "Использование: $0 [опции]"
        echo ""
        echo "Опции:"
        echo "  --help, -h     Показать эту справку"
        echo "  --build-only   Только собрать проект"
        echo "  --benchmark-only Только запустить бенчмарк"
        echo "  --plot-only    Только построить график"
        echo ""
        echo "По умолчанию выполняется полный цикл: сборка -> бенчмарк -> график"
        exit 0
        ;;
    --build-only)
        check_dependencies
        build_project
        print_success "Сборка завершена"
        exit 0
        ;;
    --benchmark-only)
        run_benchmark
        exit 0
        ;;
    --plot-only)
        plot_results
        exit 0
        ;;
    "")
        main
        ;;
    *)
        print_error "Неизвестная опция: $1"
        echo "Используйте --help для получения справки"
        exit 1
        ;;
esac 