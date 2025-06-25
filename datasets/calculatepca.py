import numpy as np
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA
import tkinter as tk
from tkinter import filedialog
import pandas as pd

def load_file():
    root = tk.Tk()
    root.withdraw()  # Скрываем основное окно
    
    file_path = filedialog.askopenfilename(
        title="Выберите файл с данными",
        filetypes=(("CSV и TXT файлы", "*.csv *.txt"), ("Все файлы", "*.*")))
    
    if not file_path:
        print("Файл не выбран. Программа завершена.")
        return None
    
    try:
        # Пробуем прочитать как CSV с разделителем запятая
        data = pd.read_csv(file_path, delimiter=',', header=None).values
        # Если не получилось, пробуем пробел/таб для txt
        if data.shape[1] == 1:
            data = pd.read_csv(file_path, delim_whitespace=True, header=None).values
        return data
    except Exception as e:
        print(f"Ошибка при чтении файла: {e}")
        return None

def perform_pca(data):
    # PCA с проекцией на 2 главные компоненты
    pca = PCA(n_components=2)
    projected = pca.fit_transform(data)

    # Вывод информации
    print("\nРезультаты PCA:")
    print("Среднее:", np.mean(data, axis=0))
    print("Ковариационная матрица:\n", np.cov(data.T))
    print("Собственные значения:", pca.explained_variance_)
    print("Собственные векторы (компоненты):\n", pca.components_)
    print("\nПроецированные данные:\n", projected)

    # ==== Регрессия между PC1 и PC2 ====
    x = projected[:, 0]
    y = projected[:, 1]

    # Добавляем столбец единиц для bias
    X_design = np.vstack([x, np.ones_like(x)]).T
    coeffs, _, _, _ = np.linalg.lstsq(X_design, y, rcond=None)  # [slope, intercept]

    slope, intercept = coeffs
    print(f"\nЛинейная регрессия: PC2 ≈ {slope:.3f} * PC1 + {intercept:.3f}")

    # Построим линию регрессии
    x_line = np.linspace(x.min(), x.max(), 100)
    y_line = slope * x_line + intercept

    # ==== График ====
    plt.figure(figsize=(8, 6))
    plt.scatter(x, y, c='blue', s=50, label='Проекция PCA')
    plt.plot(x_line, y_line, 'r--', linewidth=2, label='Линия регрессии')

    plt.title("PCA: проекция и регрессия")
    plt.xlabel(f"PC1 ({pca.explained_variance_ratio_[0]*100:.1f}%)")
    plt.ylabel(f"PC2 ({pca.explained_variance_ratio_[1]*100:.1f}%)")
    plt.legend()
    plt.grid(True)
    plt.axis('equal')
    plt.tight_layout()
    plt.show()

def main():
    print("Пожалуйста, выберите файл с данными...")
    data = load_file()
    
    if data is not None:
        print("\nЗагруженные данные:")
        print(data)
        perform_pca(data)

if __name__ == "__main__":
    main()