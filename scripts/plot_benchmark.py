#!/usr/bin/env python3
"""
Скрипт для построения графиков результатов бенчмарка производительности
студенческого реестра.
"""

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
from pathlib import Path
import os

def plot_benchmark_results(csv_file='docs/benchmark_results.csv'):
    """
    Строит графики результатов бенчмарка.
    
    Args:
        csv_file (str): Путь к CSV файлу с результатами
    """
    # Проверяем существование файла
    if not Path(csv_file).exists():
        print(f"Ошибка: Файл {csv_file} не найден!")
        print("Сначала запустите бенчмарк: ./src/benchmark")
        return
    
    # Читаем данные
    try:
        df = pd.read_csv(csv_file)
        print(f"Загружено {len(df)} записей из {csv_file}")
    except Exception as e:
        print(f"Ошибка при чтении файла: {e}")
        return
    
    # Настройка стиля
    plt.style.use('seaborn-v0_8')
    sns.set_palette("husl")
    
    # Создаём фигуру с подграфиками
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(16, 12))
    fig.suptitle('Бенчмарк производительности студенческого реестра\n'
                'Сравнение однопоточного и многопоточного режимов', 
                fontsize=16, fontweight='bold')
    
    # График 1: Время выполнения vs Размер данных (линейная шкала)
    ax1.plot(df['Students'], df['SingleThread(ms)'], 'o-', 
             label='Однопоточный режим', linewidth=3, markersize=8, color='red')
    ax1.plot(df['Students'], df['MultiThread(ms)'], 's-', 
             label='Многопоточный режим', linewidth=3, markersize=8, color='blue')
    ax1.set_xlabel('Количество студентов')
    ax1.set_ylabel('Время выполнения (мс)')
    ax1.set_title('Время выполнения vs Размер данных (линейная шкала)')
    ax1.legend(fontsize=12)
    ax1.grid(True, alpha=0.3)
    
    # Добавляем аннотации с точными значениями для больших объёмов
    for i, row in df.iterrows():
        if row['Students'] >= 100000:
            ax1.annotate(f'{row["SingleThread(ms)"]:.0f}мс', 
                        (row['Students'], row['SingleThread(ms)']),
                        textcoords="offset points", xytext=(0,10), ha='center', fontsize=10)
            ax1.annotate(f'{row["MultiThread(ms)"]:.0f}мс', 
                        (row['Students'], row['MultiThread(ms)']),
                        textcoords="offset points", xytext=(0,-15), ha='center', fontsize=10)
    
    # График 2: Ускорение vs Размер данных
    ax2.plot(df['Students'], df['Speedup'], 'o-', 
             color='green', linewidth=3, markersize=8)
    ax2.axhline(y=1, color='red', linestyle='--', alpha=0.7, label='Без ускорения')
    ax2.axhline(y=2, color='orange', linestyle='--', alpha=0.7, label='2x ускорение')
    ax2.axhline(y=4, color='purple', linestyle='--', alpha=0.7, label='4x ускорение (идеал)')
    ax2.set_xlabel('Количество студентов')
    ax2.set_ylabel('Ускорение (x)')
    ax2.set_title('Ускорение vs Размер данных')
    ax2.legend(fontsize=12)
    ax2.grid(True, alpha=0.3)
    ax2.set_xscale('log')
    ax2.set_ylim(0, 3)
    
    # Добавляем аннотации с точными значениями ускорения
    for i, row in df.iterrows():
        if row['Speedup'] > 1.5:
            ax2.annotate(f'{row["Speedup"]:.2f}x', 
                        (row['Students'], row['Speedup']),
                        textcoords="offset points", xytext=(0,10), ha='center', fontsize=10,
                        bbox=dict(boxstyle="round,pad=0.3", facecolor="yellow", alpha=0.7))
    
    # График 3: Разница во времени выполнения
    time_diff = df['SingleThread(ms)'] - df['MultiThread(ms)']
    ax3.bar(df['Students'], time_diff, color='orange', alpha=0.7, width=df['Students']*0.6)
    ax3.set_xlabel('Количество студентов')
    ax3.set_ylabel('Экономия времени (мс)')
    ax3.set_title('Экономия времени многопоточного режима')
    ax3.grid(True, alpha=0.3)
    ax3.set_xscale('log')
    
    # Добавляем подписи значений
    for i, (students, diff) in enumerate(zip(df['Students'], time_diff)):
        if diff > 0:
            ax3.annotate(f'{diff:.0f}мс', 
                        (students, diff),
                        textcoords="offset points", xytext=(0,5), ha='center', fontsize=9)
    
    # График 4: Эффективность параллелизма
    ideal_speedup = 4.0
    efficiency = df['Speedup'] / ideal_speedup * 100
    
    bars = ax4.bar(df['Students'], efficiency, color='purple', alpha=0.7, width=df['Students']*0.6)
    ax4.axhline(y=100, color='red', linestyle='--', alpha=0.7, label='100% эффективность')
    ax4.axhline(y=50, color='orange', linestyle='--', alpha=0.7, label='50% эффективность')
    ax4.axhline(y=25, color='yellow', linestyle='--', alpha=0.7, label='25% эффективность')
    ax4.set_xlabel('Количество студентов')
    ax4.set_ylabel('Эффективность параллелизма (%)')
    ax4.set_title('Эффективность параллелизма')
    ax4.legend(fontsize=10)
    ax4.grid(True, alpha=0.3)
    ax4.set_xscale('log')
    ax4.set_ylim(0, 70)
    
    # Добавляем подписи значений эффективности
    for i, (students, eff) in enumerate(zip(df['Students'], efficiency)):
        if eff > 30:
            ax4.annotate(f'{eff:.1f}%', 
                        (students, eff),
                        textcoords="offset points", xytext=(0,5), ha='center', fontsize=9,
                        bbox=dict(boxstyle="round,pad=0.2", facecolor="lightblue", alpha=0.7))
    
    # Настройка макета
    plt.tight_layout()
    
    # Сохранение графика в папку docs
    output_file = 'docs/benchmark_results.png'
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"График сохранён в файл: {output_file}")
    
    # Показ графика
    plt.show()
    
    # Создаём дополнительный график с фокусом на большие объёмы данных
    create_focused_plot(df)
    
    # Вывод статистики
    print("\n=== Статистика бенчмарка ===")
    print(f"Максимальное ускорение: {df['Speedup'].max():.2f}x")
    print(f"Минимальное ускорение: {df['Speedup'].min():.2f}x")
    print(f"Среднее ускорение: {df['Speedup'].mean():.2f}x")
    print(f"Медианное ускорение: {df['Speedup'].median():.2f}x")
    
    # Анализ эффективности
    print(f"\n=== Анализ эффективности ===")
    for _, row in df.iterrows():
        students = row['Students']
        speedup = row['Speedup']
        efficiency = speedup / 4.0 * 100  # Предполагаем 4 ядра
        time_saved = row['SingleThread(ms)'] - row['MultiThread(ms)']
        print(f"{students:>8} студентов: ускорение {speedup:>5.2f}x, эффективность {efficiency:>5.1f}%, экономия {time_saved:>6.0f}мс")

def create_focused_plot(df):
    """
    Создаёт дополнительный график с фокусом на большие объёмы данных.
    """
    # Фильтруем данные для больших объёмов
    large_data = df[df['Students'] >= 50000].copy()
    
    if len(large_data) == 0:
        return
    
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))
    fig.suptitle('Детальный анализ производительности на больших объёмах данных', 
                fontsize=16, fontweight='bold')
    
    # График 1: Время выполнения для больших объёмов
    ax1.plot(large_data['Students'], large_data['SingleThread(ms)'], 'o-', 
             label='Однопоточный режим', linewidth=3, markersize=10, color='red')
    ax1.plot(large_data['Students'], large_data['MultiThread(ms)'], 's-', 
             label='Многопоточный режим', linewidth=3, markersize=10, color='blue')
    ax1.set_xlabel('Количество студентов')
    ax1.set_ylabel('Время выполнения (мс)')
    ax1.set_title('Время выполнения на больших объёмах данных')
    ax1.legend(fontsize=12)
    ax1.grid(True, alpha=0.3)
    
    # Добавляем точные значения
    for _, row in large_data.iterrows():
        ax1.annotate(f'{row["SingleThread(ms)"]:.0f}мс', 
                    (row['Students'], row['SingleThread(ms)']),
                    textcoords="offset points", xytext=(0,15), ha='center', fontsize=11,
                    bbox=dict(boxstyle="round,pad=0.3", facecolor="lightcoral", alpha=0.8))
        ax1.annotate(f'{row["MultiThread(ms)"]:.0f}мс', 
                    (row['Students'], row['MultiThread(ms)']),
                    textcoords="offset points", xytext=(0,-20), ha='center', fontsize=11,
                    bbox=dict(boxstyle="round,pad=0.3", facecolor="lightblue", alpha=0.8))
    
    # График 2: Ускорение для больших объёмов
    bars = ax2.bar(large_data['Students'], large_data['Speedup'], 
                   color=['green' if x > 2 else 'orange' if x > 1.5 else 'red' for x in large_data['Speedup']],
                   alpha=0.7, width=large_data['Students']*0.3)
    ax2.axhline(y=1, color='red', linestyle='--', alpha=0.7, label='Без ускорения')
    ax2.axhline(y=2, color='orange', linestyle='--', alpha=0.7, label='2x ускорение')
    ax2.set_xlabel('Количество студентов')
    ax2.set_ylabel('Ускорение (x)')
    ax2.set_title('Ускорение на больших объёмах данных')
    ax2.legend(fontsize=12)
    ax2.grid(True, alpha=0.3)
    ax2.set_ylim(0, 3)
    
    # Добавляем подписи значений ускорения
    for i, (students, speedup) in enumerate(zip(large_data['Students'], large_data['Speedup'])):
        color = 'white' if speedup > 1.5 else 'black'
        ax2.annotate(f'{speedup:.2f}x', 
                    (students, speedup),
                    textcoords="offset points", xytext=(0,5), ha='center', fontsize=12, fontweight='bold',
                    color=color)
    
    plt.tight_layout()
    
    # Сохранение дополнительного графика
    output_file = 'docs/benchmark_results_large_data.png'
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"Дополнительный график сохранён в файл: {output_file}")
    
    plt.show()

def create_detailed_report(csv_file='docs/benchmark_results.csv'):
    """
    Создаёт детальный отчёт по результатам бенчмарка.
    
    Args:
        csv_file (str): Путь к CSV файлу с результатами
    """
    if not Path(csv_file).exists():
        return
    
    df = pd.read_csv(csv_file)
    
    # Создаём отчёт
    report = []
    report.append("# Отчёт по бенчмарку производительности студенческого реестра")
    report.append("")
    report.append("## Общая статистика")
    report.append(f"- Количество тестовых размеров: {len(df)}")
    report.append(f"- Диапазон данных: {df['Students'].min():,} - {df['Students'].max():,} студентов")
    report.append(f"- Максимальное ускорение: {df['Speedup'].max():.2f}x")
    report.append(f"- Минимальное ускорение: {df['Speedup'].min():.2f}x")
    report.append(f"- Среднее ускорение: {df['Speedup'].mean():.2f}x")
    report.append("")
    
    report.append("## Детальные результаты")
    report.append("| Студентов | Однопоточный (мс) | Многопоточный (мс) | Ускорение | Эффективность | Экономия времени |")
    report.append("|-----------|-------------------|-------------------|-----------|---------------|------------------|")
    
    for _, row in df.iterrows():
        efficiency = row['Speedup'] / 4.0 * 100
        time_saved = row['SingleThread(ms)'] - row['MultiThread(ms)']
        report.append(f"| {row['Students']:,} | {row['SingleThread(ms)']:.1f} | "
                     f"{row['MultiThread(ms)']:.1f} | {row['Speedup']:.2f}x | {efficiency:.1f}% | {time_saved:.0f}мс |")
    
    report.append("")
    report.append("## Ключевые выводы")
    
    # Анализ трендов
    if df['Speedup'].iloc[-1] > df['Speedup'].iloc[0]:
        report.append("- ✅ **Ускорение растёт с увеличением объёма данных**")
    else:
        report.append("- ❌ Ускорение не растёт с увеличением объёма данных")
    
    if df['Speedup'].max() > 2.0:
        report.append(f"- ✅ **Достигнуто значительное ускорение ({df['Speedup'].max():.2f}x)**")
    else:
        report.append("- ⚠️ Ускорение умеренное (<2x)")
    
    # Анализ эффективности
    max_efficiency = (df['Speedup'].max() / 4.0) * 100
    report.append(f"- 📊 **Максимальная эффективность параллелизма: {max_efficiency:.1f}%**")
    
    # Рекомендации
    report.append("")
    report.append("## Рекомендации")
    report.append("- Для небольших объёмов данных (<10,000 студентов) многопоточность не даёт преимуществ")
    report.append("- Для средних объёмов данных (10,000-100,000 студентов) ускорение умеренное")
    report.append("- Для больших объёмов данных (>100,000 студентов) многопоточность показывает значительное ускорение")
    report.append("- Оптимальное использование многопоточности начинается с ~50,000 студентов")
    
    # Сохранение отчёта в папку docs
    with open('docs/benchmark_report.md', 'w', encoding='utf-8') as f:
        f.write('\n'.join(report))
    
    print("Детальный отчёт сохранён в файл: docs/benchmark_report.md")

if __name__ == "__main__":
    print("=== Анализ результатов бенчмарка ===")
    
    # Построение графиков
    plot_benchmark_results()
    
    # Создание отчёта
    create_detailed_report()
    
    print("\nАнализ завершён!") 