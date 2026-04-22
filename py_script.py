import tensorflow as tf
import numpy as np

# Генерация синтетических данных
def generate_data(n_samples=1000):
    X = []
    y = []
    for _ in range(n_samples):
        # 0: покой – маленькая амплитуда
        if np.random.rand() < 0.33:
            acc = np.random.normal(0, 0.2, 3)
            label = 0
        # 1: ходьба – средняя амплитуда
        elif np.random.rand() < 0.5:
            acc = np.random.normal(0, 1.0, 3)
            label = 1
        # 2: бег – высокая амплитуда
        else:
            acc = np.random.normal(0, 2.5, 3)
            label = 2
        X.append(acc)
        y.append(label)
    return np.array(X, dtype=np.float32), np.array(y, dtype=np.int32)

X_train, y_train = generate_data(5000)
X_val, y_val = generate_data(1000)

# Создание модели
model = tf.keras.Sequential([
    tf.keras.layers.Input(shape=(3,)),
    tf.keras.layers.Dense(16, activation='relu'),
    tf.keras.layers.Dense(8, activation='relu'),
    tf.keras.layers.Dense(3, activation='softmax')
])

model.compile(optimizer='adam',
              loss='sparse_categorical_crossentropy',
              metrics=['accuracy'])

model.fit(X_train, y_train, validation_data=(X_val, y_val), epochs=10)

# Конвертация в TFLite
converter = tf.lite.TFLiteConverter.from_keras_model(model)
tflite_model = converter.convert()

with open('model.tflite', 'wb') as f:
    f.write(tflite_model)

print("Модель сохранена как model.tflite")
