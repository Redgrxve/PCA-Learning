import numpy as np
import pandas as pd
import os

def generate_realistic_dataset(filename: str,
                                n_samples: int = 100,
                                n_features: int = 5,
                                noise_level: float = 0.3,
                                seed: int = None):
    """
    Генерация реалистичного датасета с заголовками признаков.

    :param filename: Имя файла (в папке datasets), например 'realistic_data.csv'
    :param n_samples: Количество строк (наблюдений)
    :param n_features: Количество признаков
    :param noise_level: Уровень шума (0 — без шума)
    :param seed: Фиксировать генератор случайных чисел
    """
    if seed is not None:
        np.random.seed(seed)

    os.makedirs("datasets2", exist_ok=True)

    # Генерация скрытых "истинных" факторов
    latent_factors = np.random.randn(n_samples, 2)  # 2 латентных признака

    # Каждый признак — это линейная комбинация факторов + шум
    weights = np.random.randn(2, n_features)
    data = latent_factors @ weights

    noise = np.random.normal(loc=0.0, scale=noise_level, size=data.shape)
    data += noise

    # Создание заголовков
    columns = [f"Feature{i+1}" for i in range(n_features)]

    df = pd.DataFrame(data, columns=columns)
    df.to_csv(os.path.join("datasets2", filename), index=False)

    print(f"✅ Датасет сохранён в datasets2/{filename}")

# Пример использования
generate_realistic_dataset("realistic_data3.csv", n_samples=50, n_features=30, noise_level=0.5, seed=98)

