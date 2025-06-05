import random
import math
import time

DIGITS = {
    0: [
        "00111100",
        "01000010",
        "10000001",
        "10000001",
        "10000001",
        "10000001",
        "01000010",
        "00111100",
    ],
    1: [
        "00010000",
        "00110000",
        "01010000",
        "00010000",
        "00010000",
        "00010000",
        "00010000",
        "01111100",
    ],
    2: [
        "00111100",
        "01000010",
        "00000010",
        "00000100",
        "00001000",
        "00010000",
        "00100000",
        "01111110",
    ],
    3: [
        "00111100",
        "01000010",
        "00000010",
        "00011100",
        "00000010",
        "00000010",
        "01000010",
        "00111100",
    ],
    4: [
        "00000100",
        "00001100",
        "00010100",
        "00100100",
        "01000100",
        "01111110",
        "00000100",
        "00000100",
    ],
    5: [
        "01111110",
        "01000000",
        "01000000",
        "01111100",
        "00000010",
        "00000010",
        "01000010",
        "00111100",
    ],
    6: [
        "00111100",
        "01000010",
        "01000000",
        "01111100",
        "01000010",
        "01000010",
        "01000010",
        "00111100",
    ],
    7: [
        "01111110",
        "00000010",
        "00000100",
        "00001000",
        "00010000",
        "00100000",
        "00100000",
        "00100000",
    ],
    8: [
        "00111100",
        "01000010",
        "01000010",
        "00111100",
        "01000010",
        "01000010",
        "01000010",
        "00111100",
    ],
    9: [
        "00111100",
        "01000010",
        "01000010",
        "00111110",
        "00000010",
        "00000010",
        "01000010",
        "00111100",
    ],
}

INPUT_SIZE = 64
EPOCHS = 1000
LR = 0.1


def make_dataset():
    X = []
    y = []
    for digit, pattern in DIGITS.items():
        arr = [int(c) for row in pattern for c in row]
        X.append(arr)
        y.append(digit)
    return X, y


def dot(a, b):
    return sum(x * y for x, y in zip(a, b))


def softmax(scores):
    m = max(scores)
    exps = [math.exp(s - m) for s in scores]
    s = sum(exps)
    return [e / s for e in exps]


def train(X, y):
    random.seed(0)
    classes = len(set(y))
    W = [[random.uniform(-0.01, 0.01) for _ in range(INPUT_SIZE)] for _ in range(classes)]
    b = [0.0 for _ in range(classes)]
    start = time.time()
    for _ in range(EPOCHS):
        for xi, yi in zip(X, y):
            scores = [dot(w, xi) + b[j] for j, w in enumerate(W)]
            probs = softmax(scores)
            for j in range(classes):
                error = probs[j] - (1 if yi == j else 0)
                for k in range(INPUT_SIZE):
                    W[j][k] -= LR * error * xi[k]
                b[j] -= LR * error
    t = time.time() - start
    return W, b, t


def predict(model, X):
    W, b = model
    preds = []
    for xi in X:
        scores = [dot(W[j], xi) + b[j] for j in range(len(W))]
        probs = softmax(scores)
        preds.append(probs.index(max(probs)))
    return preds


if __name__ == '__main__':
    X, y = make_dataset()
    W, b, t = train(X, y)
    preds = predict((W, b), X)
    print('pred:', preds)
    print('truth:', y)
    print('training time: {:.3f}s'.format(t))
