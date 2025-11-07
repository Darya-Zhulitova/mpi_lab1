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
Все замеры проводились для `10000000` точек
<img width="2370" height="1765" alt="time_vs_process_count" src="https://github.com/Darya-Zhulitova/mpi_lab1/blob/main/task1/results/time_vs_process_count.png" />
<img width="2370" height="1765" alt="speedup_vs_process_count" src="https://github.com/Darya-Zhulitova/mpi_lab1/blob/main/task1/results/speedup_vs_process_count.png" />
<img width="2370" height="1765" alt="efficiency_vs_process_count" src="https://github.com/Darya-Zhulitova/mpi_lab1/blob/main/task1/results/efficiency_vs_process_count.png" />
