## Установка зависимостей
```
sudo apt update  
sudo apt install openmpi-bin openmpi-doc libopenmpi-dev python3-pandas python3-matplotlib -y
```

# Задание 1 (Жулитова Дарья)
Написать параллельную программу, которая будет вычислять число π методом Монте-Карло с использованием MPI. Построить графики времени выполнения, ускорения и эффективности.

## Запуск программы
```
mpicc -o task1 task1.c
mpirun -np <количество процессов> ./task1 <количество точек>
```
## Результат
Все замеры проводились для `10000000` точек.
<img width="2370" height="1765" alt="time_vs_process_count" src="https://github.com/Darya-Zhulitova/mpi_lab1/blob/main/task1/results/time_vs_process_count.png" />
<img width="2370" height="1765" alt="speedup_vs_process_count" src="https://github.com/Darya-Zhulitova/mpi_lab1/blob/main/task1/results/speedup_vs_process_count.png" />
<img width="2370" height="1765" alt="efficiency_vs_process_count" src="https://github.com/Darya-Zhulitova/mpi_lab1/blob/main/task1/results/efficiency_vs_process_count.png" />

# Задание 2 (Федоренко Артём)
Реализовать алгоритмы для умножения матрицы на вектор, используя разбиение по строкам, по столбцам и по блокам. Провести численные эксперименты, замеряя время выполнения функций при различных размерах входных данных. Построить графики времени выполнения, ускорения и эффективности.

## Запуск программы
```
mpicc -o <block/row/col> <block/row/col>.c
mpirun -np <количество процессов> ./<block/row/col> <высота матрицы> <ширина матрицы>
```
## Результат
Все замеры проводились для матрицы `10000x10000`.
### Разбиение по строкам
<img width="2370" height="1765" alt="time_vs_process_count" src="https://github.com/Darya-Zhulitova/mpi_lab1/blob/main/task2/result/row.csv_time_vs_process_count.png" />
<img width="2370" height="1765" alt="speedup_vs_process_count" src="https://github.com/Darya-Zhulitova/mpi_lab1/blob/main/task2/result/row.csv_speedup_vs_process_count.png" />
<img width="2370" height="1765" alt="efficiency_vs_process_count" src="https://github.com/Darya-Zhulitova/mpi_lab1/blob/main/task2/result/row.csv_efficiency_vs_process_count.png" />

### Разбиение по столбцам
<img width="2370" height="1765" alt="time_vs_process_count" src="https://github.com/Darya-Zhulitova/mpi_lab1/blob/main/task2/result/col.csv_time_vs_process_count.png" />
<img width="2370" height="1765" alt="speedup_vs_process_count" src="https://github.com/Darya-Zhulitova/mpi_lab1/blob/main/task2/result/col.csv_speedup_vs_process_count.png" />
<img width="2370" height="1765" alt="efficiency_vs_process_count" src="https://github.com/Darya-Zhulitova/mpi_lab1/blob/main/task2/result/col.csv_efficiency_vs_process_count.png" />

### Разбиение по блокам
<img width="2370" height="1765" alt="time_vs_process_count" src="https://github.com/Darya-Zhulitova/mpi_lab1/blob/main/task2/result/block.csv_time_vs_process_count.png" />
<img width="2370" height="1765" alt="speedup_vs_process_count" src="https://github.com/Darya-Zhulitova/mpi_lab1/blob/main/task2/result/block.csv_speedup_vs_process_count.png" />
<img width="2370" height="1765" alt="efficiency_vs_process_count" src="https://github.com/Darya-Zhulitova/mpi_lab1/blob/main/task2/result/block.csv_efficiency_vs_process_count.png" />
