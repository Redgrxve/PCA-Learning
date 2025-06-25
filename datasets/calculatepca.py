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

    # График
    plt.figure(figsize=(8, 6))
    plt.scatter(projected[:, 0], projected[:, 1], c='blue', s=50)
    plt.title("PCA: проекция на 2 главные компоненты")
    plt.xlabel(f"PC1 ({pca.explained_variance_ratio_[0]*100:.1f}%)")
    plt.ylabel(f"PC2 ({pca.explained_variance_ratio_[1]*100:.1f}%)")
    plt.grid(True)
    plt.axis('equal')
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