import numpy as np
import pandas as pd
import os

# Создаём папку datasets, если её нет
os.makedirs("datasets", exist_ok=True)

low = -10.0
high = 10.0
noise_level = 0.5       # обычный разброс
y_noise_level = 5.0   

#разброс по диагонали
for i in range(1, 3):
    base_values = np.random.uniform(low, high, size=(100, 1))
    data = np.repeat(base_values, 5, axis=1)

    # Шум: общий и отдельный для Y
    noise = np.random.normal(loc=0.0, scale=noise_level, size=(100, 5))
    noise[:, 1] = np.random.normal(loc=0.0, scale=y_noise_level, size=100)  # увеличенный шум по Y

    data += noise

    df = pd.DataFrame(data)
    df.to_csv(f"datasets/random_dataset_{i}.csv", index=False, header=False)

#обычный разброс
for i in range(4, 7):
    data = np.random.uniform(low, high, size=(10, 3))  # 100 строк, 5 признаков
    df = pd.DataFrame(data)
    df.to_csv(f"datasets/random_dataset_{i}.csv", index=False, header=False)