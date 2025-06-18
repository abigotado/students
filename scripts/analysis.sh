#!/bin/bash

# Скрипт для статического и динамического анализа кода
# Student Registry Project

set -e

echo "=== Анализ кода проекта Student Registry ==="
echo

# Создаём директорию для анализа
mkdir -p build_analysis
cd build_analysis

echo "1. Статический анализ с Clang..."
cmake .. -DCMAKE_BUILD_TYPE=Debug
make clean
make 2>&1 | tee static_analysis.log

echo
echo "2. Анализ на утечки памяти с Address Sanitizer..."
cmake .. -DCMAKE_BUILD_TYPE=Debug -DENABLE_ADDRESS_SANITIZER=ON
make clean
make 2>&1 | tee address_sanitizer.log

echo
echo "3. Анализ многопоточности с Thread Sanitizer..."
cmake .. -DCMAKE_BUILD_TYPE=Debug -DENABLE_THREAD_SANITIZER=ON
make clean
make 2>&1 | tee thread_sanitizer.log

echo
echo "4. Запуск тестов с Address Sanitizer..."
if [ -f "./src/student_app" ]; then
    echo "Тестирование приложения на утечки памяти..."
    timeout 30s ./src/student_app < /dev/null || true
fi

if [ -f "./tests/run_tests" ]; then
    echo "Запуск модульных тестов..."
    ./tests/run_tests 2>&1 | tee tests_with_sanitizer.log
fi

echo
echo "5. Анализ с помощью cppcheck (если установлен)..."
if command -v cppcheck &> /dev/null; then
    cppcheck --enable=all --std=c++20 --language=c++ ../libs/ ../src/ ../tests/ 2>&1 | tee cppcheck.log
else
    echo "cppcheck не установлен. Установите: brew install cppcheck"
fi

echo
echo "=== Анализ завершён ==="
echo "Логи сохранены в build_analysis/"
echo
echo "Результаты:"
echo "- static_analysis.log - Статический анализ компилятора"
echo "- address_sanitizer.log - Анализ утечек памяти"
echo "- thread_sanitizer.log - Анализ многопоточности"
echo "- tests_with_sanitizer.log - Результаты тестов с санитайзерами"
if command -v cppcheck &> /dev/null; then
    echo "- cppcheck.log - Анализ cppcheck"
fi 