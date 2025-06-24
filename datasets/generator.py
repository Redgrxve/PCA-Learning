import numpy as np
import pandas as pd

data = np.random.rand(100, 5)  # 100 строк, 5 признаков
df = pd.DataFrame(data, columns=[f'Feature{i}' for i in range(1, 6)])
df.to_csv("random_dataset.csv", index=False, header=False)