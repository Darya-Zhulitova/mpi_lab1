import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import sys
import os

filename = sys.argv[1]
data = pd.read_csv(filename)

baseline_time = data['t'].iloc[0]
baseline_processes = data['n'].iloc[0]
data['s'] = baseline_time / data['t']
data['e'] = data['s'] / data['n']

plt.style.use('default')

plt.figure(figsize=(8, 6))
plt.plot(data['n'], data['t'], 'bo-', linewidth=2, markersize=8)
plt.xlabel('Количество процессов')
plt.ylabel('Время (мс)')
plt.title('Зависимость времени выполнения от количества процессов')
plt.grid(True, alpha=0.3)
plt.xticks(data['n'])
plt.tight_layout()
plt.savefig(filename + '_time_vs_process_count.png', dpi=300, bbox_inches='tight')
plt.close()

plt.figure(figsize=(8, 6))
plt.plot(data['n'], data['s'], 'ro-', linewidth=2, markersize=8)
plt.plot(data['n'], data['n'], 'k--', linewidth=1, alpha=0.7)
plt.xlabel('Количество процессов')
plt.ylabel('Коэффициент ускорения')
plt.title('Зависимость коэффициента ускорения от количества процессов')
plt.grid(True, alpha=0.3)
plt.legend()
plt.xticks(data['n'])
plt.tight_layout()
plt.savefig(filename + '_speedup_vs_process_count.png', dpi=300, bbox_inches='tight')
plt.close()

plt.figure(figsize=(8, 6))
plt.plot(data['n'], data['e'], 'go-', linewidth=2, markersize=8)
plt.axhline(y=1, color='k', linestyle='--', alpha=0.7)
plt.xlabel('Количество процессов')
plt.ylabel('Эффективность')
plt.title('Зависимость эффективности от количества процессов')
plt.grid(True, alpha=0.3)
plt.legend()
plt.xticks(data['n'])
plt.ylim(0, 1.2)
plt.tight_layout()
plt.savefig(filename + '_efficiency_vs_process_count.png', dpi=300, bbox_inches='tight')
plt.close()