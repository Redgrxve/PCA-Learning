import numpy as np
import pandas as pd

# Задаём диапазон случайных чисел
low = -10.0     # минимальное значение
high = 10.0     # максимальное значение

# Генерация 5 файлов
for i in range(1, 6):
    data = np.random.uniform(low, high, size=(100, 5))  # 100 строк, 5 признаков
    df = pd.DataFrame(data)
    df.to_csv(f"random_dataset_{i}.csv", index=False, header=False)
